# Telegram-stealer
Account stealer for telegram

# How 2 use
1.compile builder for the stealer ( Or use precompiled )
2.open builder and insert token and chat id ( place templater.rs along with builder )
3.build stealer
4.wait for victim to open exe
5.tdata will be send to assigned group id
6.download telegram portable
7.copy tdata to telegram portable folder
8.open telegram portable
9.Yay! you are in account
 [ if victim terminate session, you will no longer be able to log into the victim's account ]

# How does this work?
victim opening exe, and after this programm reads tdata folder in "%appdata%/Telegram Desktop/" and after reading, adding file to zip and sending to telegram
