USE HospitalDB;

SELECT
  p.Patient_ID,
  p.Patient_FName,
  p.Patient_LName,
  ROUND(SUM(m.M_Cost * pr.Dosage),2) AS MonthlyMedicineCost
FROM Prescription pr
JOIN Patient   p ON pr.Patient_ID  = p.Patient_ID
JOIN Medicine  m ON pr.Medicine_ID = m.Medicine_ID
WHERE pr.Date >= '2025-05-01' 
  AND pr.Date <  '2025-06-01'
GROUP BY
  p.Patient_ID,
  p.Patient_FName,
  p.Patient_LName
ORDER BY
  MonthlyMedicineCost DESC
LIMIT 5;


--Identifies highest-cost medicine consumers.