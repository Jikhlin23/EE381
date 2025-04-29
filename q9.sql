USE HospitalDB;

-- Compute joined per month
WITH Joined AS (
  SELECT
    DATE_FORMAT(Date_Joining, '%Y-%m') AS Month,
    COUNT(*) AS JoinedCount
  FROM Staff
  WHERE Date_Joining BETWEEN '2025-04-01' AND '2025-05-31'
  GROUP BY Month
),
Separated AS (
  SELECT
    DATE_FORMAT(Date_Seperation, '%Y-%m') AS Month,
    COUNT(*) AS SeparatedCount
  FROM Staff
  WHERE Date_Seperation BETWEEN '2025-04-01' AND '2025-05-31'
  GROUP BY Month
)
SELECT
  j.Month,
  j.JoinedCount   AS Joined,
  COALESCE(s.SeparatedCount, 0) AS Separated
FROM Joined j
LEFT JOIN Separated s
  ON j.Month = s.Month
ORDER BY j.Month;

-- --Counts staff joining and separation by month.
