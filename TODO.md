#TRANSMITTER
###Main thread
1. Mengirim data dalam bentuk frame

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
        jika buf ke i tidak kosong
            baca buf ke i
            lakukan checksum
            if checksum sesuai
                kirim ACK
            else
                kirim NAK
end loop
```
