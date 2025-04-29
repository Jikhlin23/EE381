USE HospitalDB;

WITH stays AS (
  SELECT
    r.Room_Type,
    DATEDIFF(MAX(ap.Date), MIN(ap.Date)) + 1 AS StayDays,
    ap.Patient_ID,
    r.Room_ID
  FROM Appointment ap
  JOIN Room r
    ON ap.Patient_ID = r.Patient_ID
  GROUP BY
    r.Room_ID,
    ap.Patient_ID
)
SELECT
  Room_Type,
  ROUND(AVG(StayDays),1) AS AvgStayDays
FROM stays
GROUP BY
  Room_Type
ORDER BY
  AvgStayDays DESC;


--Uses a CTE to estimate how long patients remain in each room type.