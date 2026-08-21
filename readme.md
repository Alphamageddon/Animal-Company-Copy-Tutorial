## THIS VERSION IS BAD AND OUTDATED USE THE NEW ONE:
https://github.com/Xera-Games-LLC/Xera-Company-Backend



https://github.com/Xera-Games-LLC/Xera-Company-Backend




https://github.com/Xera-Games-LLC/Xera-Company-Backend



https://github.com/Xera-Games-LLC/Xera-Company-Backend












## Old Readme for people who insist on using this shitty backend:

CREDITS:

Xera - Developer  
1. Install Base APK & Gamedata
- Use QuestAppVersionSwitcher to get target APK version
- Go to the /gamedata folder in this repo and download the wanted game data (only a few are available)

2. Decompile APK
- Use APKToolGUI or similar to unpack

3. Patch Server URL
- Open global-metadata.dat in MetaDataStringEditor
- Ctrl+F:
  https://animalcompany.us-east1.nakamacloud.io
- Replace with:
  https://ac-xerabackend.pythonanywhere.com or your hosted backend url if self hosting

- Or go to /native-lib patch native-lib.cpp for your url and recompile (THIS IS THE ONLY WAY TO DO IT ON NEWER VERSIONS PAST LAVA)

4. Inject App ID
- Open assets/bin/data/globalgamemanagers in UABEA
- Ctrl+F: OculusPlatformSettings ? Extract as raw
- Open extracted .dat in HxD
- Replace Meta App ID with your own from developer.oculus.com

5. Rebrand Package Name
- Change:
  woosterGames.animalCompany
- Edit in AndroidManifest.xml and apktool.yml

6. Upload to Meta
- Sign in to developer.oculus.com
- Build and sign APK
- Upload to App Lab
