

type Interval = (Double, Double)

-- polynomial in monomial base with coefficients in little endian
-- (Monomials []) behaves exactly as (Monomials [0]) but the second is to be prefered if feasible
data Monomials = Monomials { mCoeffs :: [Double] } deriving (Show, Eq, Ord) 

-- polynomial in bernstein base relative to given interval with coefficients in little endian
-- the bernstein base is not normalized, meaning that the base polynomial B_k^n 
-- multiplied by its coefficient at index k in a list with n+1 elements 
-- is written as B_k^n(x) = x^k * (1-x)^(n-k) relative to interval (0, 1)
-- and as        B_k^n(x) = (b - a)^(-n) * (x - a)^k * (b - x)^(n-k) relative to interval (a, b)
data Bernstein = Bernstein { bCoeffs :: [Double],
                             interval :: Interval } deriving (Show, Eq, Ord) 

-- list of coefficients in unspecified base
type PolyList = [Double] 

combineWith :: (Double -> Double -> Double) -> PolyList -> PolyList -> PolyList
combineWith f     []     ys = ys
combineWith f     xs     [] = xs
combineWith f (x:xs) (y:ys) = (f x y) : (combineWith f xs ys)

instance Num Monomials where 
    (+) xs ys     = Monomials (combineWith (+) (mCoeffs xs) (mCoeffs ys))
    (-) xs ys     = Monomials (combineWith (-) (mCoeffs xs) (mCoeffs ys))
    (*) xs ys     = Monomials (mathFold (mCoeffs xs) (mCoeffs ys))
            where mathFold     [] ys = []
                  mathFold (x:xs) ys = combineWith (+) (map (*x) ys) (0.0 : (mathFold xs ys))
    negate xs     = Monomials $ map negate $ mCoeffs xs
    abs xs        = Monomials [sqrt $ (sum . map (^2) . mCoeffs) xs]
    signum xs     = Monomials [] -- undefinded
    fromInteger i = Monomials [fromInteger i]

-- constructs a polynomial in monomial base from real roots
monomialsFromRoots :: [Double] -> Monomials
monomialsFromRoots roots = foldr (*) (Monomials [1]) [Monomials [-root, 1] | root <- roots]

-- constructs a polynomial in monomial base from complex root pairs
-- only one half of each complex pair is given explicitly in the input e.g. (a, b) represents roots a+bi and a-bi
monomialsFromComplexRootPairs :: [(Double, Double)] -> Monomials
monomialsFromComplexRootPairs rootPairs = foldr (*) (Monomials [1]) factorsList
    where factorsList = [Monomials [a^2 + b^2, -2*a, 1] | (a, b) <- rootPairs]

-- uses horner scheme to evaluate the polynomial at x    
monomialsEvaluate :: Monomials -> Double -> Double
monomialsEvaluate ms x = foldr (\m val -> m + x * val) 0.0 (mCoeffs ms)

bernsteinEvaluate :: Bernstein -> Double -> Double
bernsteinEvaluate bs x = (sum $ map (\(k, b) -> b * evalBase_kn k) (zip [0..] $ bCoeffs bs)) / (b - a)^n
    where evalBase_kn k = monomialsEvaluate ((Monomials [-a, 1])^k * (Monomials [b, -1])^(n - k)) x
          (a, b) = interval bs
          n = length $ bCoeffs bs

-- base transformation
bernsteinFromMonomials :: Monomials -> Interval -> Bernstein
bernsteinFromMonomials ms interval = Bernstein (bernsteinCoeffsFromMonomials ms interval) interval

-- base transformation helper (also used in descartes algorithm)
bernsteinCoeffsFromMonomials :: Monomials -> Interval -> PolyList
bernsteinCoeffsFromMonomials ms (a, b) = foldr (combineWith (+)) [0] $ map (mCoeffs . toSummand) (zip [0..] $ mCoeffs ms)
    where toSummand (k, m) = (Monomials [1, 1])^(n - k) * (Monomials [a, b])^k * (Monomials [m])
                   where n = length $ mCoeffs ms
















