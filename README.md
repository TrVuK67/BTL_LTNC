# Cat & Dog

- [Giới thiệu về bản thân](#giới-thiệu-về-bản-thân)
- [Giới thiệu về Game](#Giới-thiệu-về-Game)
- [Hướng dẫn chơi Game](#Hướng-dẫn-chơi-Game)
- [Một vài hình ảnh của Game](#Một-vài-hình-ảnh-của-Game)
- [Cấu trúc Game](#cấu-trúc-)
- [Cách cài đặt](#Cách-cài-đặt)
- [Nguồn tham khảo](#Nguồn-tham-khảo)
- [Kết luận](#Kết-luận)
  	- [Ưu điểm](#Ưu-điểm)
   	- [Nhược điểm](#Nhược-điểm)
   	- [Bài học kinh nghiệm](#khó-khăn-và-bài-học)
   	- [Hướng phát triển Game trong tương lai](#Hướng-phát-triển-Game-trong-tương-lai)
   	- [Nguyện vọng về điểm](#nguyện-vọng-về-điểm)





## Giới thiệu về bản thân
•	Họ và tên: Trần Hoàng Vũ

• Mã sinh viên: 22028014

•	Lớp học phần: 2324II_INT2215_70


## Giới thiệu về Game
Với trò chơi "Cat vs. Dog"! Đây là một trò chơi 2 người đơn giản, nơi hai nhân vật - Cat và Dog - đã xảy ra xích mích nên ném đồ vật vào nhau qua bức tường rào. Mục tiêu của trò chơi là làm giảm thanh máu của đối phương xuống 0 và giành chiến thắng. Trò chơi được xây dựng bằng C++ và sử dụng thư viện SDL2 để xử lý đồ họa và âm thanh.

Video giới thiệu về game: https://drive.google.com/file/d/1Y1LWmVIWS-tJ-WU1zESRFlufPoF-QyiH/view


## Hướng dẫn chơi Game
Bằng cách kéo thả chuột bạn sẽ điều khiển quỹ đạo ném của nhân vật sao cho bay qua hàng rào và trúng đối phương . Mỗi lượt người chơi chỉ được phép ném một lần nếu ném trúng hay trượt cũng sẽ đều đổi lượt .


## Thông tin về trò chơi
•	![Bone](https://github.com/TrVuK67/BTL_LTNC/blob/main/Bone.png?raw=true): Là đạo cụ để Dog ném ra.

•	![Can](https://github.com/TrVuK67/BTL_LTNC/blob/main/Can.png?raw=true): Là đạo cụ để Cat ném ra.

•	![wall](https://github.com/TrVuK67/BTL_LTNC/blob/main/wall.png?raw=true): Bức tường ngăn cách 2 nhân vật .
 
• 	![CAT](https://github.com/TrVuK67/BTL_LTNC/blob/main/CAT.png?raw=true): Nhân vật Cat .

• 	![DOG](https://github.com/TrVuK67/BTL_LTNC/blob/main/DOG.png?raw=true): Nhân vật Dog .



## Một vài hình ảnh của Game

![cat_laugh](https://github.com/TrVuK67/BTL_LTNC/blob/main/cat_laugh.png?raw=true)

![dog_laugh](https://github.com/TrVuK67/BTL_LTNC/blob/main/dog_laugh.png?raw=true)

![background](https://github.com/TrVuK67/BTL_LTNC/blob/main/background.png?raw=true)




## Cấu trúc .
•	angle(): Xử lí quỹ đạo vật thể ném ra.

•	showMainMenu(): Menu game.

•	checkCollision(): kiểm tra hitbox khi ném. 

•	initSDL() : khởi tạo . 


## Cách cài đặt.

Yêu Cầu Hệ Thống:
Hệ điều hành: Windows, macOS, hoặc Linux
C++ Compiler (như GCC, Clang, hoặc MSVC)
SDL2, SDL2_image, SDL2_ttf, và SDL2_mixer libraries
Font file ("Font.ttf") và các file hình ảnh âm thanh yêu cầu khác
Cài Đặt SDL2 và Các Thư Viện Liên Quan:
Tải SDL2 và các thư viện liên quan:

SDL2
SDL2_image
SDL2_ttf
SDL2_mixer
Cài đặt thư viện:

Giải nén các thư viện và sao chép các file .dll, .dylib, hoặc .so vào thư mục hệ thống của bạn hoặc thư mục dự án.
Cấu hình dự án:

Đảm bảo các file header và thư viện của SDL2, SDL2_image, SDL2_ttf, và SDL2_mixer được cấu hình đúng trong dự án của bạn.
Biên Dịch và Chạy Game:
Biên dịch game:

Sử dụng lệnh biên dịch phù hợp với compiler của bạn. Ví dụ với GCC:
bash
Copy code
g++ -o catndog main.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
Đảm bảo rằng các thư viện SDL2 và các thư viện liên quan đã được liên kết đúng cách.
Chạy game:

Chạy file thực thi đã biên dịch:
bash
Copy code
./catndog


## Kỹ thuật, công nghệ sử dụng
•	Sử dụng các tính năng trong thư viện SDL bao gồm: xử lý đồ họa, font chữ, âm thanh, thao tác chuột, bàn phím, …

•	Sử dụng công thức vật lí để tạo quỹ đạo đường đạn.

•	Xử lí va chạm .


## Nguồn tham khảo
•	Hướng dẫn xây dựng : web Lazy Foo' .

•	Ý tưởng : Game Gunny . 

•	Âm thanh : Youtube . 

•	Hình ảnh : Phim hoạt hình . 
## Kết luận

### Ưu điểm
•	Game đã phát triển được giao diện đồ họa cơ bản.

•	Game có thể chơi cùng bạn bè người thân . 

•	Hình ảnh nhân vật hài hước . 


### Nhược điểm
•	Giao diện đồ họa âm thanh còn sơ sài.

•	chưa có nút bấm dung chơi khi trong trò chơi .

### Khó khăn và bài học
•	Thuật toán logic cho vật thể ném ra phức tạp.

•	Còn gặp khó khăn trong việc chia file

•	Biết sử dụng các công cụ hỗ trợ của thư viện SDL.

•	Biết cách sử dung Git.

### Hướng phát triển Game trong tương lai
•	Thêm vật cản trên không , thanh gió , v..v

•	kỹ năng bổ trợ ( + máu , phóng to vật ném , v...v...)

•	thêm máy tự động chơi ( 1 player mode ) .

### Nguyện vọng về điểm
Mức điểm mong muốn 8.5+/10.

