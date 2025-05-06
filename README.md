
# Project NINJA

## 📚 Project Description

**Project NINJA** is a console-based Library Management System written in C, designed for educational purposes. It provides functionality for both **admin** and **user** roles, allowing for book catalog management, borrowing/returning of books, and waitlist management. Data is stored using `.csv` files, and all significant actions are logged for audit purposes.

Main features include:
- Admin: Add/remove books, view all books, check borrow history, manage queues.
- User: Search books, borrow/return, join waitlist for unavailable books.
- Persistent file storage for accounts, books, borrow records, and queues.
- Logging system that records every major action with timestamps.

---

## ▶️ How to Run the Code

### 🛠 Compile the program

Use a C compiler (e.g., GCC) to compile all `.c` files. Make sure you're in the project folder with all source files and `.csv` data files present.

```bash
<<<<<<< HEAD
gcc sourcecode/*.c -o run
=======
gcc main.c login.c admin.c user.c readsavefiles.c algorithm.c log.c -o run
>>>>>>> 9d5fdef49f41841ae7c1d49cb4b7b5bd7f115a04
```

If you're on Windows and using MinGW:

```bash
<<<<<<< HEAD
gcc sourcecode/*.c -o run.exe
=======
gcc main.c login.c admin.c user.c readsavefiles.c algorithm.c log.c -o run.exe
>>>>>>> 9d5fdef49f41841ae7c1d49cb4b7b5bd7f115a04
```

### 📂 Required files

Ensure the following `.csv` files are in the same folder as your compiled program:

- `account.csv`
- `booklist.csv`
- `borrow.csv`
- `queue.csv`

Example starter admin account in `account.csv`:
```
id,username,password,isadmin
0,admin,admin,1
```

### 🚀 Run the program

After compiling, run the executable:

```bash
<<<<<<< HEAD
./run      # On Linux/macOS
=======
./run       # On Linux/macOS
>>>>>>> 9d5fdef49f41841ae7c1d49cb4b7b5bd7f115a04
run.exe     # On Windows
```

Login with:
```
Username: admin
Password: admin
```

Then follow the on-screen menu for admin or user operations.

---

## 💡 Sample Input and Expected Output

### 🧪 Sample Input: Admin logs in and adds a book

```plaintext
Username: admin
Password: admin

==============
Admin Dashboard
==========\\

[1] Manage Books
[2] Manage Queue
[3] Manage Accounts
[4] View Log
[0] Logout
---------------------
Select an option: 1



==============
Admin Dashboard
==========\\

[1] Manage Books
[2] Manage Queue
[3] Manage Accounts
[4] View Log
[0] Logout
---------------------
Select an option: 1



==============
Manage Books
==========\\


NUM   Title                               Author                         ISBN                 Available
--------------------------------------------------------------------------------------------------------
[1 ] 1984                                George Orwell                  9780452284234        Yes
[2 ] Animal Farm                         George Orwell                  9780451526342        Yes
[3 ] Anna Karenina                       Leo Tolstoy                    9780140449174        Yes
[4 ] Attack on Titan                     Hajime Isayama                 9781632365684        Yes
[5 ] Attack on Titan: Before the Fall    Ryo Suzukaze & Satoshi Shiki   9781975356449        Yes
[6 ] Berserk                             Kentaro Miura                  9781593071993        Yes
[7 ] Black Butler                        Yana Toboso                    9781421537748        Yes
[8 ] Black Clover                        Y┼½ki Tabata                   9781974710533        Yes
[9 ] Bleach                              Tite Kubo                      9781421524442        Yes
[10] Bleach: Thousand-Year Blood War     Tite Kubo                      9781975354452        Yes
[11] Blue Exorcist                       Kazue Kato                     9781421567896        Yes
[12] Brave New World                     Aldous Huxley                  9780060850524        Yes
[13] Catch-22                            Joseph Heller                  9781451626650        Yes
[14] Chainsaw Man                        Tatsuki Fujimoto               9781632366518        Yes
[15] Chainsaw Man Vol. 2 supplemental    Tatsuki Fujimoto               9781632366525        Yes
[16] City Hunter                         Tsukasa Hojo                   9784088723346        Yes
[17] Crime and Punishment                Fyodor Dostoevsky              9780140449136        Yes
[18] Crows                               Hiroshi Takahashi              9784088720932        Yes
[19] D.Gray-man                          Kaori Yuki                     9781421517696        Yes
[20] Death Note                          Tsugumi Ohba & Takeshi Obata   9781421501689        Yes
[21] Demon Slayer: Hashira Training Arc  Koyoharu Got┼ìge               9781632366402        Yes
[22] Demon Slayer: Kimetsu no Yaiba      Koyoharu Got┼ìge               9781974702161        Yes
[23] Dr. Stone                           Riichir┼ì Inagaki & Boichi     9781974714968        Yes
[24] Dragon Ball                         Akira Toriyama                 9781569312739        Yes
[25] Fahrenheit 451                      Ray Bradbury                   9781451673319        Yes
[26] Fairy Tail                          Hiro Mashima                   9781632362614        Yes
[27] Fist of the North Star              Buronson & Tetsuo Hara         9781569314276        Yes
[28] Fullmetal Alchemist                 Hiromu Arakawa                 9781591167121        Yes
[29] Great Expectations                  Charles Dickens                9780141439563        Yes
[30] Haikyu!!                            Haruichi Furudate              9781975334353        Yes
[31] Hunter ├ù Hunter                    Yoshihiro Togashi              9781591168128        Yes
[32] Jane Eyre                           Charlotte Bront├½              9780142437209        Yes
[33] Jujutsu Kaisen                      Gege Akutami                   9781974713060        Yes
[34] Kinnikuman                          Yudetamago                     9784088720581        Yes
[35] Les Mis├⌐rables                     Victor Hugo                    9780451419439        Yes
[36] Lolita                              Vladimir Nabokov               9780679723165        Yes
[37] Minami no Tei┼ì                     Dai Tenn┼ìji & Rikiya G┼ì      9784091280810        Yes
[38] Mob Psycho 100                      ONE & Y┼½suke Murata           9781421595751        Yes
[39] Moby-Dick                           Herman Melville                9780142437247        Yes
[40] My Hero Academia                    K┼ìhei Horikoshi               9781974701935        Yes
[41] Naruto                              Masashi Kishimoto              9781421500842        Yes
[42] Naruto Shipp┼½den                   Masashi Kishimoto              9781421518365        Yes
[43] One Hundred Years of Solitude       Gabriel Garc├¡a M├írquez       9780060883287        Yes
[44] One Piece                           Eiichiro Oda                   9781591160368        Yes
[45] Pride and Prejudice                 Jane Austen                    9780141439518        Yes
[46] Slam Dunk                           Takehiko Inoue                 9781591165219        Yes
[47] Soul Eater                          Atsushi ┼îkubo                 9781421501849        Yes
[48] Spy ├ù Family                       Tatsuya End┼ì                  9781975360279        Yes
[49] The Brothers Karamazov              Fyodor Dostoevsky              9780374528379        Yes
[50] The Catcher in the Rye              J.D. Salinger                  9780316769488        Yes
[51] The Divine Comedy                   Dante Alighieri                9780140448955        Yes
[52] The Great Gatsby                    F. Scott Fitzgerald            9780743273565        Yes
[53] The Hobbit                          J.R.R. Tolkien                 9780547928227        Yes
[54] The Kite Runner                     Khaled Hosseini                9781594631931        Yes
[55] The Lord of the Rings               J.R.R. Tolkien                 9780544003415        Yes
[56] The Odyssey                         Homer                          9780140268867        Yes
[57] The Promised Neverland              Kaiu Shirai & Posuka Demizu    9781421597243        Yes
[58] The Seven Deadly Sins               Nakaba Suzuki                  9781974703151        Yes
[59] To Kill a Mockingbird               Harper Lee                     9780060935467        Yes
[60] Tokyo Ghoul                         Sui Ishida                     9781421596482        Yes
[61] Tokyo Revengers                     Ken Wakui                      9781632366457        Yes
[62] Vagabond                            Takehiko Inoue                 9781591166308        Yes
[63] Vinland Saga                        Makoto Yukimura                9781632364492        Yes
[64] War and Peace                       Leo Tolstoy                    9780199232765        Yes
[65] Wuthering Heights                   Emily Bront├½                  9780141439556        Yes
[66] Your Lie in April                   Naoshi Arakawa                 9781421591272        Yes
[67] Yu-Gi-Oh!                           Kazuki Takahashi               9781569311848        Yes

Total books: 67

[1] Sort
[2] Search
[3] Add
[4] Delete
[5] Edit
[0] Return
Select option: 5


==============
Add New Book
==========\\


Title (0 to cancel): How to basic
Author: kenny
ISBN: 67070503419
Availability (1=Available, 0=Unavailable): 1
```

**Expected Output:**

```plaintext
Book added successfully!
[Log entry in library.log]
06/05/2025 15:52:03 - Added new book: How to basic



NUM   Title                               Author                         ISBN                 Available
--------------------------------------------------------------------------------------------------------
[1 ] How to basic                        kenny                          67070503419          Yes

```

---

### 🧪 Sample Input: User logs in and borrows a book

```plaintext
Username: user01
Password: pass123




NUM   Title                               Author                         ISBN                 Available 
--------------------------------------------------------------------------------------------------------
[1 ] How to basic                        kenny                          67070503419          Yes       
[2 ] 1984                                George Orwell                  9780452284234        Yes       
[3 ] Animal Farm                         George Orwell                  9780451526342        Yes       
[4 ] Anna Karenina                       Leo Tolstoy                    9780140449174        Yes       
[5 ] Attack on Titan                     Hajime Isayama                 9781632365684        Yes       
[6 ] Attack on Titan: Before the Fall    Ryo Suzukaze & Satoshi Shiki   9781975356449        Yes       
[7 ] Berserk                             Kentaro Miura                  9781593071993        Yes       
[8 ] Black Butler                        Yana Toboso                    9781421537748        Yes       
[9 ] Black Clover                        Y┼½ki Tabata                   9781974710533        Yes       
[10] Bleach                              Tite Kubo                      9781421524442        Yes       
[11] Bleach: Thousand-Year Blood War     Tite Kubo                      9781975354452        Yes       
[12] Blue Exorcist                       Kazue Kato                     9781421567896        Yes       
[13] Brave New World                     Aldous Huxley                  9780060850524        Yes       
[14] Catch-22                            Joseph Heller                  9781451626650        Yes       
[15] Chainsaw Man                        Tatsuki Fujimoto               9781632366518        Yes       
[16] Chainsaw Man Vol. 2 supplemental    Tatsuki Fujimoto               9781632366525        Yes       
[17] City Hunter                         Tsukasa Hojo                   9784088723346        Yes       
[18] Crime and Punishment                Fyodor Dostoevsky              9780140449136        Yes       
[19] Crows                               Hiroshi Takahashi              9784088720932        Yes       
[20] D.Gray-man                          Kaori Yuki                     9781421517696        Yes       
[21] Death Note                          Tsugumi Ohba & Takeshi Obata   9781421501689        Yes       
[22] Demon Slayer: Hashira Training Arc  Koyoharu Got┼ìge               9781632366402        Yes       
[23] Demon Slayer: Kimetsu no Yaiba      Koyoharu Got┼ìge               9781974702161        Yes       
[24] Dr. Stone                           Riichir┼ì Inagaki & Boichi     9781974714968        Yes       
[25] Dragon Ball                         Akira Toriyama                 9781569312739        Yes       
[26] Fahrenheit 451                      Ray Bradbury                   9781451673319        Yes       
[27] Fairy Tail                          Hiro Mashima                   9781632362614        Yes       
[28] Fist of the North Star              Buronson & Tetsuo Hara         9781569314276        Yes       
[29] Fullmetal Alchemist                 Hiromu Arakawa                 9781591167121        Yes       
[30] Great Expectations                  Charles Dickens                9780141439563        Yes       
[31] Haikyu!!                            Haruichi Furudate              9781975334353        Yes       
[32] Hunter ├ù Hunter                    Yoshihiro Togashi              9781591168128        Yes
[33] Jane Eyre                           Charlotte Bront├½              9780142437209        Yes
[34] Jujutsu Kaisen                      Gege Akutami                   9781974713060        Yes
[35] Kinnikuman                          Yudetamago                     9784088720581        Yes
[36] Les Mis├⌐rables                     Victor Hugo                    9780451419439        Yes
[37] Lolita                              Vladimir Nabokov               9780679723165        Yes
[38] Minami no Tei┼ì                     Dai Tenn┼ìji & Rikiya G┼ì      9784091280810        Yes
[39] Mob Psycho 100                      ONE & Y┼½suke Murata           9781421595751        Yes
[40] Moby-Dick                           Herman Melville                9780142437247        Yes
[41] My Hero Academia                    K┼ìhei Horikoshi               9781974701935        Yes
[42] Naruto                              Masashi Kishimoto              9781421500842        Yes
[43] Naruto Shipp┼½den                   Masashi Kishimoto              9781421518365        Yes
[44] One Hundred Years of Solitude       Gabriel Garc├¡a M├írquez       9780060883287        Yes
[45] One Piece                           Eiichiro Oda                   9781591160368        Yes
[46] Pride and Prejudice                 Jane Austen                    9780141439518        Yes
[47] Slam Dunk                           Takehiko Inoue                 9781591165219        Yes
[48] Soul Eater                          Atsushi ┼îkubo                 9781421501849        Yes
[49] Spy ├ù Family                       Tatsuya End┼ì                  9781975360279        Yes
[50] The Brothers Karamazov              Fyodor Dostoevsky              9780374528379        Yes
[51] The Catcher in the Rye              J.D. Salinger                  9780316769488        Yes
[52] The Divine Comedy                   Dante Alighieri                9780140448955        Yes
[53] The Great Gatsby                    F. Scott Fitzgerald            9780743273565        Yes
[54] The Hobbit                          J.R.R. Tolkien                 9780547928227        Yes
[55] The Kite Runner                     Khaled Hosseini                9781594631931        Yes
[56] The Lord of the Rings               J.R.R. Tolkien                 9780544003415        Yes
[57] The Odyssey                         Homer                          9780140268867        Yes
[58] The Promised Neverland              Kaiu Shirai & Posuka Demizu    9781421597243        Yes
[59] The Seven Deadly Sins               Nakaba Suzuki                  9781974703151        Yes
[60] To Kill a Mockingbird               Harper Lee                     9780060935467        Yes
[61] Tokyo Ghoul                         Sui Ishida                     9781421596482        Yes
[62] Tokyo Revengers                     Ken Wakui                      9781632366457        Yes
[63] Vagabond                            Takehiko Inoue                 9781591166308        Yes
[64] Vinland Saga                        Makoto Yukimura                9781632364492        Yes
[65] War and Peace                       Leo Tolstoy                    9780199232765        Yes
[66] Wuthering Heights                   Emily Bront├½                  9780141439556        Yes
[67] Your Lie in April                   Naoshi Arakawa                 9781421591272        Yes
[68] Yu-Gi-Oh!                           Kazuki Takahashi               9781569311848        Yes

Total books: 68

==============
Main Menu [ID: 1]
==============

[1] Sort Books
[2] Search Books
[3] Borrow Books
[4] Return Books
[5] My Queue
[0] Logout
-------------------------------
Select an option: 3




NUM   Title                               Author                         ISBN                 Available 
--------------------------------------------------------------------------------------------------------
[1 ] How to basic                        kenny                          67070503419          Yes       
[2 ] 1984                                George Orwell                  9780452284234        Yes       
[3 ] Animal Farm                         George Orwell                  9780451526342        Yes       
[4 ] Anna Karenina                       Leo Tolstoy                    9780140449174        Yes       
[5 ] Attack on Titan                     Hajime Isayama                 9781632365684        Yes       
[6 ] Attack on Titan: Before the Fall    Ryo Suzukaze & Satoshi Shiki   9781975356449        Yes       
[7 ] Berserk                             Kentaro Miura                  9781593071993        Yes       
[8 ] Black Butler                        Yana Toboso                    9781421537748        Yes       
[9 ] Black Clover                        Y┼½ki Tabata                   9781974710533        Yes       
[10] Bleach                              Tite Kubo                      9781421524442        Yes       
[11] Bleach: Thousand-Year Blood War     Tite Kubo                      9781975354452        Yes       
[12] Blue Exorcist                       Kazue Kato                     9781421567896        Yes       
[13] Brave New World                     Aldous Huxley                  9780060850524        Yes       
[14] Catch-22                            Joseph Heller                  9781451626650        Yes       
[15] Chainsaw Man                        Tatsuki Fujimoto               9781632366518        Yes       
[16] Chainsaw Man Vol. 2 supplemental    Tatsuki Fujimoto               9781632366525        Yes       
[17] City Hunter                         Tsukasa Hojo                   9784088723346        Yes       
[18] Crime and Punishment                Fyodor Dostoevsky              9780140449136        Yes       
[19] Crows                               Hiroshi Takahashi              9784088720932        Yes       
[20] D.Gray-man                          Kaori Yuki                     9781421517696        Yes       
[21] Death Note                          Tsugumi Ohba & Takeshi Obata   9781421501689        Yes       
[22] Demon Slayer: Hashira Training Arc  Koyoharu Got┼ìge               9781632366402        Yes       
[23] Demon Slayer: Kimetsu no Yaiba      Koyoharu Got┼ìge               9781974702161        Yes       
[24] Dr. Stone                           Riichir┼ì Inagaki & Boichi     9781974714968        Yes
[25] Dragon Ball                         Akira Toriyama                 9781569312739        Yes
[26] Fahrenheit 451                      Ray Bradbury                   9781451673319        Yes
[27] Fairy Tail                          Hiro Mashima                   9781632362614        Yes
[28] Fist of the North Star              Buronson & Tetsuo Hara         9781569314276        Yes
[29] Fullmetal Alchemist                 Hiromu Arakawa                 9781591167121        Yes
[30] Great Expectations                  Charles Dickens                9780141439563        Yes
[31] Haikyu!!                            Haruichi Furudate              9781975334353        Yes
[32] Hunter ├ù Hunter                    Yoshihiro Togashi              9781591168128        Yes
[33] Jane Eyre                           Charlotte Bront├½              9780142437209        Yes
[34] Jujutsu Kaisen                      Gege Akutami                   9781974713060        Yes
[35] Kinnikuman                          Yudetamago                     9784088720581        Yes
[36] Les Mis├⌐rables                     Victor Hugo                    9780451419439        Yes
[37] Lolita                              Vladimir Nabokov               9780679723165        Yes
[38] Minami no Tei┼ì                     Dai Tenn┼ìji & Rikiya G┼ì      9784091280810        Yes
[39] Mob Psycho 100                      ONE & Y┼½suke Murata           9781421595751        Yes
[40] Moby-Dick                           Herman Melville                9780142437247        Yes
[41] My Hero Academia                    K┼ìhei Horikoshi               9781974701935        Yes
[42] Naruto                              Masashi Kishimoto              9781421500842        Yes
[43] Naruto Shipp┼½den                   Masashi Kishimoto              9781421518365        Yes
[44] One Hundred Years of Solitude       Gabriel Garc├¡a M├írquez       9780060883287        Yes
[45] One Piece                           Eiichiro Oda                   9781591160368        Yes
[46] Pride and Prejudice                 Jane Austen                    9780141439518        Yes
[47] Slam Dunk                           Takehiko Inoue                 9781591165219        Yes
[48] Soul Eater                          Atsushi ┼îkubo                 9781421501849        Yes
[49] Spy ├ù Family                       Tatsuya End┼ì                  9781975360279        Yes
[50] The Brothers Karamazov              Fyodor Dostoevsky              9780374528379        Yes
[51] The Catcher in the Rye              J.D. Salinger                  9780316769488        Yes
[52] The Divine Comedy                   Dante Alighieri                9780140448955        Yes
[53] The Great Gatsby                    F. Scott Fitzgerald            9780743273565        Yes
[54] The Hobbit                          J.R.R. Tolkien                 9780547928227        Yes
[55] The Kite Runner                     Khaled Hosseini                9781594631931        Yes
[56] The Lord of the Rings               J.R.R. Tolkien                 9780544003415        Yes
[57] The Odyssey                         Homer                          9780140268867        Yes
[58] The Promised Neverland              Kaiu Shirai & Posuka Demizu    9781421597243        Yes
[59] The Seven Deadly Sins               Nakaba Suzuki                  9781974703151        Yes
[60] To Kill a Mockingbird               Harper Lee                     9780060935467        Yes
[61] Tokyo Ghoul                         Sui Ishida                     9781421596482        Yes
[62] Tokyo Revengers                     Ken Wakui                      9781632366457        Yes
[63] Vagabond                            Takehiko Inoue                 9781591166308        Yes
[64] Vinland Saga                        Makoto Yukimura                9781632364492        Yes
[65] War and Peace                       Leo Tolstoy                    9780199232765        Yes
[66] Wuthering Heights                   Emily Bront├½                  9780141439556        Yes
[67] Your Lie in April                   Naoshi Arakawa                 9781421591272        Yes
[68] Yu-Gi-Oh!                           Kazuki Takahashi               9781569311848        Yes

Total books: 68

[0] Cancel
[1-68] Select book
Select an option: 1

Confirm borrow 'How to basic'? (Y/N): Y
Enter borrowing days (1-30): 15
```
**Expected Output:**

```plaintext
Book borrowed successfully!
[Log entry in library.log]
06/05/2025 16:00:45 - User 1 borrowed: How to basic



==============
Return Books [ID: 1]
==============

NUM   ISBN                 Title                               Borrow Date     Return Date
------------------------------------------------------------------------------------------------
[1 ] 67070503419          How to basic                        06/05/2025      21/05/2025

[0] Cancel
[1-1] Select book to return

```
