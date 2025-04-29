USE HospitalDB;

SELECT
  doc.Doctor_ID,
  s.Emp_FName,
  s.Emp_LName
FROM Doctor doc
JOIN Staff s
  ON doc.Emp_ID = s.Emp_ID
WHERE doc.Doctor_ID NOT IN (
  SELECT DISTINCT ap.Doctor_ID
  FROM Appointment ap
  WHERE ap.Date >= DATE_SUB(CURDATE(), INTERVAL 30 DAY)
);



-- Finds idle doctors to reassign or investigate.