#TRANSMITTER
###Main thread
1. Baca file text, masukkan ke dalam memori.
2. Buat array of frame, isikan datanya dari file text tadi.
3. Mengirim data dalam bentuk frame sebanyak lebar window

###Second thread
1. Menerima ACK.
    * Jika pengiriman berhasil, bisa melakukan pengiriman selanjutnya
2. Menerima NAK
    * Jika pengiriman tidak berhasil, kirim ulang frame tersebut.
3. Jika tidak ada ACK dalam selang waktu tertentu, kirimkan ulang frame tersebut


#RECEIVER
###Main thread
1. Menerima frame kemudian masukkan ke dalam buffer.
2. Data dimasukkan ke dalam buffer berdasarkan frame number

###Second Thread
```
loop
    for i = 1 to bufSize
        if buf ke i tidak kosong
            baca buf ke i
            lakukan checksum
            if checksum sesuai
                kirim ACK
            else
                kirim NAK
            kosongkan buf ke i
end loop
```
