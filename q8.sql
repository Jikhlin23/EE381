USE HospitalDB;

SELECT
  p.Patient_ID,
  p.Patient_FName,
  p.Patient_LName,
  mh.Allergies,
  COUNT(pr.Prescription_ID) AS RxCount
FROM Medical_History mh
JOIN Patient      p ON mh.Patient_ID = p.Patient_ID
JOIN Prescription pr ON p.Patient_ID  = pr.Patient_ID
WHERE mh.Allergies <> 'None'
GROUP BY
  p.Patient_ID,
  p.Patient_FName,
  p.Patient_LName,
  mh.Allergies
HAVING
  RxCount > 0;


--Flags patients with allergies who have active prescriptions (for review).