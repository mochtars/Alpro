#include <iostream>
#include <ctime>
#include <unistd.h>

using namespace std;
// struct untuk waktu
struct Waktu {
  int dd, mm, yy;
  int hour, minute;
  time_t test;
};
// struct untuk Parkir mobil dan motor
struct Parkir {
  Waktu waktuMasuk;
  Waktu waktuKeluar;
  string nama;
  string noPolisi;
  // char noPolisi[15];
  bool status;
};
// perhitungan waktu, 1 = 10 menit/1 detik
const int perDetik = 1;
const int totalDetik = perDetik * 6;
const int totalHari = totalDetik * 24;
// inisialiasasi struct disini agar bisa di akses secara global
Parkir parkirMobil[10] = {};
Parkir parkirMotor[10] = {};
long duit[100] = {};
long duitHarian = 0;

void menampilkanParkiran();
void menghitungParkirKosong(string jenis);
void kendaraanMasuk();
void kendaraanKeluar();
void hapusData(Parkir *kendaraan);
void membuatJamMenit(int waktu);
int hitungTotalPendapatan();
int hitungRataRataPendapatan();
int pencarianPemilikKendaraan(Parkir *kendaraan, string *nama);
int menghitungHari(int waktu);
int menghitungJam(int waktu);
int menghitungJamRaw(int waktu);
int menghitungMenit(int waktu);

time_t hello;

int main () {

  string pilihan;
  system("clear");
  hello = time(0) - totalHari;
  
  while(true) {
    // system("clear");
    cout << "|==> Management Parkir <==|\n";
    cout << "-> Hari ke " << menghitungHari(time(0) - hello) << endl;
    cout << "-> Pukul " << menghitungJam(time(0) - hello);
    cout << ":" << menghitungMenit(time(0) - hello) << endl;
    cout << "-> Pendapatan hari ini: Rp " << duit[menghitungHari(time(0) - hello)] << "\n";
    cout << "-> Total pendapatan: Rp " << hitungTotalPendapatan() << "\n";
    cout << "-> Rata-rata pendapatan: Rp " << hitungRataRataPendapatan() << "\n\n";
    cout << "|=> Program Parkir Motor dan Mobil <=|\n";
    cout << "1. Menampilkan parkiran\n2. Masuk ke parkiran\n3. Keluar dari parkiran\n0. Keluar Program\n";
    cout << "Jadi, sekarang anda mau apa? ";
    // debug 
    // sleep(1);
    // continue;
    cin >> pilihan;

    cin.ignore(100, '\n');
    if (pilihan == "1") menampilkanParkiran(); 
    else if (pilihan == "2") kendaraanMasuk();
    else if (pilihan == "3") kendaraanKeluar();
    else if (pilihan == "0") break;
    else {
      cout << "Tolong masukan input yang sesuai!\n";
      sleep(1);
      system("clear");
      // error handling jika user menginputkan lebih dari 1 karakter
    }
  }


  return 0;
}

void menampilkanParkiran() {
  system("clear");

  cout << "=============    PARKIR    =============\n";
  cout << "==> Mobil <==             ==> Motor <==\n";

  for (short i = 0; i < 10; i++) {
    for (short j = 0; j <= 20; j++) {
      if (j == 0) {
        string current = (parkirMobil[i].status) ? parkirMobil[i].nama : "-----";
        cout << "[" << i << "] # " << current;
        cout << "(" << menghitungJam(parkirMobil[i].waktuMasuk.test);
        cout << ":" << menghitungMenit(parkirMobil[i].waktuMasuk.test) << ")";
      }
      else if (j == 20) {
        string current = (parkirMotor[i].status) ? parkirMotor[i].nama : "-----";
        cout << current;
        cout << "(" << menghitungJam(parkirMotor[i].waktuMasuk.test);
        cout << ":" << menghitungMenit(parkirMotor[i].waktuMasuk.test) << ")";
        cout << " #" ;
      }
      else cout << " ";
    }
    cout << endl;
  }

  menghitungParkirKosong("mobil");
  menghitungParkirKosong("motor");
  cout << endl;
}

void menghitungParkirKosong(string jenis) {
  short result = 0;
  Parkir *kendaraan;

  if (jenis == "mobil") kendaraan = parkirMobil;
  else kendaraan = parkirMotor;

  for (int i = 0; i < 10; i++) {
    if (!kendaraan[i].status) result++;
  }

  cout << "Parkir " << jenis << " yang kosong: " << result << "\n";
}
void kendaraanMasuk() {
  Parkir *kendaraan;
  string nama;
  string noPolisi;
  string temp;
  unsigned int id;

  system("clear");
  cout << "==> Program Parkir Motor dan Mobil <==\n";
  cout << "-> Masukan nama: ";
  cin >> nama;
  cout << "-> Masukan nomor polisi: ";
  cin >> noPolisi;

  // Memilih jenis kendaraan
  system("clear");
  do {
    cout << "==> Program Parkir Motor dan Mobil <==\n";
    cout << "1. Mobil  2. Motor\nApa jenis kendaraan anda? ";
    cin >> temp;

    if (temp == "1") kendaraan = parkirMobil;
    else if (temp == "2") kendaraan = parkirMotor;
    else {
      cout << "Tolong Pilih Nomor yang valid!\n";
      sleep(1);
      system("clear");
      cin.ignore(100, '\n');
    }
  } while (temp != "1" && temp != "2");

  // Memilih tempat parkir
  do {
    system("clear");
    cout << "==> Program Parkir Motor dan Mobil <==\n";
    menampilkanParkiran();
    cout << "-> Mau parkir di nomor berapa? ";
    cin >> temp;
    // error user input handling
    try {
      id = std::stoi(temp);

      if (id < 10)  {
        if (!kendaraan[id].status) break;
        else {
          cout << "Tempat yang anda pilih sudah diisi orang!\n";
          sleep(1);
        }
      } else {
        cout << "Tolong pilih angka yang sesuai!\n";
        sleep(1);
      }
    } catch (const std::exception&) {
      cout << "Tolong Pilih Nomor yang valid!\n";
      sleep(1);
      system("clear");
    }
  } while (true);

  // konfirmasi data ke user
  while (temp != "y") {
    system("clear");
    cout << "==> Program Parkir Motor dan Mobil <==\n";
    cout << "-> Nama: " << nama << endl;
    cout << "-> Plat Nomor: " << noPolisi << endl;
    cout << "-> Jenis Kendaraan: mobil\n";
    cout << "-> Parkir di no: " << id << endl;
    cout << "Apakah data sudah sesuai (y/n)? ";
    cin >> temp;

    if (temp == "y") {
      kendaraan[id].nama = nama;
      kendaraan[id].noPolisi = noPolisi;
      kendaraan[id].status = true;
      kendaraan[id].waktuMasuk.test = time(0) - hello;

      cout << "Selamat, data anda sudah kami terima!\n";
      sleep(1);
      system("clear");
      return;
    } else if (temp == "n") {
      system("clear");
      cout << "==> Program Parkir Motor dan Mobil <==\n";
      cout << "1. Kembali mengisi form\n0. Keluar dari form\n";
      cout << "-> Jadi sekarang bagaimana? ";
      cin >> temp;
      if (temp == "1") {
        kendaraanMasuk();
        return;
      } else if (temp == "0") return;
      else {
        cout << "Tolong pilih nomor yang sesuai!\n";
        sleep(1);
      }
    } else {
      cout << "Tolong pilih y atau n\n";
      sleep(1);
    } 
  }
}
void kendaraanKeluar() {

  string nama;
  string jenis;
  string *parkirMana;
  Parkir *kendaraan;
  string temp;

  system("clear");
  cout << "==> Program Parkir Motor dan Mobil <==\n";
  cout << "Kendaraan atas nama siapa kak? ";
  cin >> nama;
  cout << " 1. Mobil  2. Motor\n";
  cout << "Jenis kendaraannya apa kak? ";
  cin >> temp;
  cout << "Baik kak, Kami cek terlebih dahulu mohon tunggu 3 detik\n";
  // Menampilkan jeda 3 detik, 1 2 3 
  for (int i = 1; i <= 3; i++) {
    cout << i << " " << flush;
    sleep(1);
  }
  cout << endl;
  // Memilih jenis
  if (temp == "1") kendaraan = parkirMobil;
  else kendaraan = parkirMotor;

  int ID = pencarianPemilikKendaraan(kendaraan, &nama);

  if (ID >= 0) {
    int jamMasuk = menghitungJamRaw(kendaraan[ID].waktuMasuk.test);
    int jamKeluar = menghitungJamRaw(time(0) - hello);
    int totalJam = jamKeluar - jamMasuk;
    long totalBiaya = (totalJam == 0) ? 2000 : totalJam * 2000;

    cout << "|----- Ditemukan data -----|\n";
    cout << "-> Nama: " << kendaraan[ID].nama << endl;
    cout << "-> Plat Nomor: " << kendaraan[ID].noPolisi << endl;
    cout << "-> Jenis Kendaraan: mobil\n";
    cout << "-> Parkir di no: " << ID << endl;
    cout << "-> Waktu parkir: ";
    membuatJamMenit(kendaraan[ID].waktuMasuk.test);
    cout << " - ";
    membuatJamMenit(time(0) - hello);
    cout << "\n-> Berapa lama: " << totalJam << " Jam" << endl;
    cout << "-> Total biaya parkir anda yang harus dibayar: Rp " << totalBiaya << endl;
    cout << "Apakah anda yakin mau keluar bos '" << nama << "'? (y/n)? ";
    cin >> temp;
    if (temp == "y") {
      int hari = menghitungHari(time(0) - hello);
      duit[hari] += totalBiaya;
      hapusData(&kendaraan[ID]);

      cout << "Selamat Anda telah berhasil keluar dari parkiran!\n";

      sleep(2);
    } else {
      system("clear");
      return;
    }
  } else {
    system("clear");
    cout << "==> Program Parkir Motor dan Mobil <==\n";
    cout << "-> tidak ditemukan atas nama '" << nama << "'" << endl;
    cout << "1. Cari kembali\n0. Kembali ke menu utama\n";
    cout << "Sekarang mau apa? ";
    cin >> temp;
    if (temp == "1") {
      kendaraanKeluar();
      return;
    } else if (temp == "0") {
      system("clear");
      return;
    }
  }
}
int pencarianPemilikKendaraan(Parkir *kendaraan, string *nama) {
  for (int i = 0; i < 10; i++) {
    if (kendaraan[i].nama == *nama) return i;
  }
  return -1;
}
void hapusData(Parkir *kendaraan) {
  kendaraan->nama = "";
  kendaraan->waktuMasuk.test = 0;
  kendaraan->noPolisi = "";
  kendaraan->status = false;
}
int hitungTotalPendapatan() {
  int result = 0;
  for (int i = 0; i < 100; i++) {
    result += duit[i];
  }
  return result;
}
int hitungRataRataPendapatan() {
  int hari = menghitungHari(time(0) - hello);
  int result = 0;

  for (int i = 0; i <= hari; i++) {
    result += duit[i];
  }
  return result / hari;
}
void membuatJamMenit(int waktu) {
  cout << menghitungJam(waktu) << ":" << menghitungMenit(waktu);
}

int menghitungHari(int waktu) {
  return waktu / totalHari;
}
int menghitungJam(int waktu) {
  return (waktu % totalHari) / totalDetik;
}
int menghitungMenit(int waktu) {
  return (waktu % totalHari) % totalDetik / perDetik * 10;
}
int menghitungJamRaw(int waktu) {
  return waktu / totalDetik;
}
