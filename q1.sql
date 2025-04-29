USE HospitalDB;

SELECT 
  doc.Specialization,
  s.Emp_FName    AS DoctorFirst,
  s.Emp_LName    AS DoctorLast,
  ap.Day_Of_Week,
  COUNT(*)       AS Appts
FROM Appointment ap
JOIN Doctor doc
  ON ap.Doctor_ID = doc.Doctor_ID
JOIN Staff s
  ON doc.Emp_ID   = s.Emp_ID
WHERE ap.Date BETWEEN '2025-05-01' AND '2025-05-31'
GROUP BY
  doc.Specialization,
  s.Emp_FName,
  s.Emp_LName,
  ap.Day_Of_Week
ORDER BY
  doc.Specialization,
  ap.Day_Of_Week;


--Shows workload by specialization and weekday.