#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <random>

using namespace std;

// jumlah parkiran yang akan dibuat
const int jumlahParkiran = 10;
// perhitungan waktu, 1 detik = 10menit (simulasi)
const float perDetik = 1;
int totalDetik = perDetik * 6;
int totalJam = totalDetik * 24;

// Total pendapatan
long duit[100] = {};
long duitHarian = 0;
// struct untuk Parkir mobil dan motor
struct Parkir {
  string nama;
  string noPolisi;
  string jenis;
  long waktu;
  bool status;
};
// inisialiasasi struct disini agar bisa di akses secara global
// Parkir Mobil && Motor
Parkir parkirMobil[jumlahParkiran] = {};
Parkir parkirMotor[jumlahParkiran] = {};
// fungsi prototype
void menampilkanParkiran();
void menampilkanManagement();
void menghitungParkirKosong();
void kendaraanMasuk();
void kendaraanKeluar();
void hapusData(Parkir *kendaraan);
void printWaktu(int waktu);
void autoPilot();
int hitungTotalPendapatan();
int hitungRataRataPendapatan();
int pencarianPemilikKendaraan(Parkir *kendaraan, string *nama);
int menghitungHari(int waktu);
int menghitungJam(int waktu);
int menghitungJamRaw(int waktu);
int menghitungMenit(int waktu);

// untuk perhitungan waktu agar dimulai dari 0 ketika program dijalankan
unsigned long hello;

int main () {

  string pilihan;
  system("clear");
  hello = time(0) - totalJam;
  for (int i = 0; i < jumlahParkiran; i++) {
    parkirMobil[i].jenis = "mobil";
    parkirMotor[i].jenis = "motor";
  }
  
  while(true) {
    // system("clear");
    menampilkanManagement();
    cout << "|=> Program Parkir Motor dan Mobil <=|\n";
    cout << "1. Menampilkan parkiran\n2. Masuk ke parkiran\n3. Keluar dari parkiran\n99. Mode Autopilot\n0. Keluar Program\n";
    cout << "Jadi, sekarang anda mau apa? ";
    // Mengambil input dari user
    cin >> pilihan;
    cin.ignore(100, '\n');
    // menentukan pilihan dari user
    if (pilihan == "1") menampilkanParkiran(); 
    else if (pilihan == "2") kendaraanMasuk();
    else if (pilihan == "3") kendaraanKeluar();
    else if (pilihan == "99") autoPilot();
    else if (pilihan == "0") break;
    else {
      cout << "Tolong masukan input yang sesuai!\n";
      sleep(1);
      system("clear");
      // error handling jika user menginputkan lebih dari 1 karakter
    }
  }

  cout << "\nTerima Kasih telah menggunakan program kami :)\n\n";

  return 0;
}

void menampilkanParkiran() {
  system("clear");

  cout << "======================= PARKIR ========================\n";
  cout << "==> Mobil <==                             ==> Motor <==\n";


  for (short i = 0; i < jumlahParkiran; i++) {
    for (short j = 0; j <= 25; j++) {
      if (j == 0) {
        string current = (parkirMobil[i].status) ? parkirMobil[i].nama : "-----";
        int jam = menghitungJam(parkirMobil[i].waktu);
        int menit = menghitungMenit(parkirMobil[i].waktu);

        cout << "[" << i << "] # ";
        cout << "(" << jam << ":" << menit << ")";
        cout << current;

        if (current.length() > 5) {
          j += parkirMobil[i].nama.length() - 5;
        }
        else if (current.length() < 5) {
          for (int ii = current.length(); ii < 5; ii++) {
            cout << " ";
          }
        } 

        if (jam < 10) cout << " ";
        if (menit < 10) cout << " ";
      }
      else if (j == 25) {
        string current = (parkirMotor[i].status) ? parkirMotor[i].nama : "-----";
        int jam = menghitungJam(parkirMotor[i].waktu);
        int menit = menghitungMenit(parkirMotor[i].waktu);

        if (current.length() < 5) {
          for (int ii = current.length(); ii < 5; ii++) {
            current.insert(0, " ");
          }
        } else if (current.length() > 5) {
          for (int ii = current.length(); ii > 5; ii--) {
            cout << "\b";
          }
        }

        if (jam >= 10) cout << "\b";
        if (menit >= 10) cout << "\b";
        
        cout << current;
        cout << "(" << jam << ":" << menit << ")";
        cout << " #";

      }
      else cout << " ";
    }
    cout << endl;
  }

  menghitungParkirKosong();
}
void menampilkanManagement() {
  cout << "|=> Management Parkir <=|\n";
  cout << "-> Hari ke " << menghitungHari(time(0) - hello) << endl;
  cout << "-> Pukul " << menghitungJam(time(0) - hello);
  cout << ":" << menghitungMenit(time(0) - hello) << endl;
  cout << "-> Pendapatan hari ini: Rp " << duit[menghitungHari(time(0) - hello)] << "\n";
  cout << "-> Total pendapatan: Rp " << hitungTotalPendapatan() << "\n";
  cout << "-> Rata-rata pendapatan: Rp " << hitungRataRataPendapatan() << "\n\n";
}
void menghitungParkirKosong() {
  short mobilKosong = 0;
  short motorKosong = 0;

  for (int i = 0; i < jumlahParkiran; i++) {
    if (!parkirMobil[i].status) mobilKosong++;
    if (!parkirMotor[i].status) motorKosong++;
  }

  cout << "Parkir mobil yang kosong: " << mobilKosong << endl;
  cout << "Parkir motor yang kosong: " << motorKosong << endl << endl;
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
  do {
    cout << "  1. Mobil  2. Motor\n-> Apa jenis kendaraan anda? ";
    cin >> temp;

    if (temp == "1") kendaraan = parkirMobil;
    else if (temp == "2") kendaraan = parkirMotor;
    else {
      cout << "Tolong Pilih Nomor yang valid!\n";
      sleep(1);
      // system("clear");
      cin.ignore(100, '\n');
    }
  } while (temp != "1" && temp != "2");

  // Memilih tempat parkir
  while (true) {
    // system("clear");
    cout << "==> Program Parkir Motor dan Mobil <==\n";
    menampilkanParkiran();
    cout << "-> Mau parkir di nomor berapa? ";
    cin >> temp;
    //  user input error handling
    try {
      id = std::stoi(temp);

      if (id < jumlahParkiran)  {
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
  }

  // konfirmasi data ke user
  while (true) {
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
      kendaraan[id].waktu = time(0) - hello;

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
  Parkir *kendaraan;
  string temp;

  system("clear");
  cout << "==> Program Parkir Motor dan Mobil <==\n";
  menampilkanParkiran();
  cout << "Kendaraan atas nama siapa kak? ";
  cin >> nama;
  // Memilih jenis
  do {
    system("clear");
    cout << "==> Program Parkir Motor dan Mobil <==\n";
    menampilkanParkiran();
    cout << "Kendaraan atas nama siapa kak? " << nama << endl;
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

  // Menampilkan jeda 3 detik, 1 2 3 
  cout << "Baik kak, Kami cek terlebih dahulu mohon tunggu 3 detik\n";
  for (int i = 1; i <= 3; i++) {
    cout << i << " " << flush;
    sleep(1);
  }

  system("clear");

  int ID = pencarianPemilikKendaraan(kendaraan, &nama);

  if (ID >= 0) {
    int jamMasuk = menghitungJamRaw(kendaraan[ID].waktu);
    int jamKeluar = menghitungJamRaw(time(0) - hello);
    int totalJam = jamKeluar - jamMasuk;
    long totalBiaya = 0;
    // perhitungan biaya parkir
    if (kendaraan == parkirMobil) totalBiaya = totalJam * 3000;
    else if (kendaraan == parkirMotor) totalBiaya = totalJam * 2000;
    else return;
    // mengatur biaya agar tidak lebih dari 20000
    if (totalBiaya > 20000) totalBiaya = 20000;

    cout << "|----- Ditemukan data -----|\n";
    cout << "-> Nama: " << kendaraan[ID].nama << endl;
    cout << "-> Plat Nomor: " << kendaraan[ID].noPolisi << endl;
    cout << "-> Jenis Kendaraan: " << kendaraan[ID].jenis << endl;
    cout << "-> Parkir di no: " << ID << endl;
    cout << "-> Waktu parkir: ";
    printWaktu(kendaraan[ID].waktu);
    cout << " - ";
    printWaktu(time(0) - hello);
    cout << "\n-> Berapa lama: " << totalJam << " Jam" << endl;
    cout << "-> Total biaya parkir anda yang harus dibayar: Rp " << totalBiaya << endl;
    cout << "Apakah anda yakin mau keluar bos '" << nama << "'? (y/n)? ";

    while (temp != "y") {
      cin >> temp;

      if (temp == "y") {
        int hari = menghitungHari(time(0) - hello);
        duit[hari] += totalBiaya;
        hapusData(&kendaraan[ID]);
        cout << "Selamat Anda telah berhasil keluar dari parkiran!\n";
        sleep(2);
        system("clear");
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
      }
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
  for (int i = 0; i < jumlahParkiran; i++) {
    if (kendaraan[i].nama == *nama) return i;
  }
  return -1;
}
void hapusData(Parkir *kendaraan) {
  kendaraan->nama = "";
  kendaraan->noPolisi = "";
  kendaraan->waktu = 0;
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
void printWaktu(int waktu) {
  cout << menghitungJam(waktu) << ":" << menghitungMenit(waktu);
}

int menghitungHari(int waktu) {
  return waktu / totalJam;
}
int menghitungJam(int waktu) {
  return (waktu % totalJam) / totalDetik;
}
int menghitungMenit(int waktu) {
  return (waktu % totalJam) % totalDetik / perDetik * 10;
}
int menghitungJamRaw(int waktu) {
  return waktu / totalDetik;
}


void autoPilot() {
  string temp;
  Parkir *kendaraan;
  int max = 0;

  const string calonCalon[] = {
    "ADI", "ALUNG", "ALYA", "ANDRY", "ARIF", "ASEP", "Alfarisi", "BENI", "Bangbang", "DERI", "DEVIN", "Dea", "Derry", "Dicky", "Galang", "Hilwa", "IRGI", "KRISNA", "Dappa", "NATAN", "NENG NOVA", "NUR KHORI", "Najwatul", "Noer Azis", "REDDI", "RIPANI", "Rafli", "Rafli", "Abdullatif", "Risma", "Roy", "Sandhy", "Sultan", "TESA", "YANA", "ZAINUDDIN",
  };

  while (max == 0) {
    try {
      cout << "\nWARNING!!!\nprogram tidak bisa dihentikan selama Autopilot!!!\nAutoPilot sampai hari ke? ";
      cin >> temp;
      max = std::stoi(temp);
    } catch (const std::exception&) {
      system("clear");
    }
    
  }

  while (temp != "y") {
    cout << "apakah anda yakin (y/n)? ";
    cin >> temp;

    if (temp == "y") break;
    else if (temp == "n") {
      system("clear");
      return;
    } else {
      cout << "Pilih y atau n!" << endl;
      sleep(1);
      system("clear");
    } 
  } 


  // random //
  std::random_device rd; // Membuat objek random_device untuk mendapatkan seed acak
  std::mt19937 gen(rd()); // Membuat generator nomor acak menggunakan seed dari random_device
  std::uniform_int_distribution<> idAcak(0, jumlahParkiran - 1); // Mendefinisikan distribusi acak antara 1 dan 10
  std::uniform_int_distribution<> calonAcak(0, 35); // Mendefinisikan distribusi acak antara 1 dan 10
  std::uniform_int_distribution<> thinking(1, 5); // Mendefinisikan distribusi acak antara 1 dan 10
  std::uniform_int_distribution<> disJenis(1, 2); // Mendefinisikan distribusi acak antara 1 dan 10
  //

  while (true) {
    if (menghitungHari(time(0) - hello) >= max) break;

    int id = idAcak(gen);
    int nama = calonAcak(gen);
    int keputusan = thinking(gen);
    int jenis = disJenis(gen);

    hello -= 1;

    if (jenis == 1) kendaraan = parkirMobil;
    if (jenis == 2) kendaraan = parkirMotor;


    if (keputusan == 1) {
      if (!kendaraan[id].status) {
        kendaraan[id].nama = calonCalon[nama];
        kendaraan[id].noPolisi = "HCHYC";
        kendaraan[id].status = true;
        kendaraan[id].waktu = time(0) - hello;
      }
    } else {
      if (kendaraan[id].status) {
        int jamMasuk = menghitungJamRaw(kendaraan[id].waktu);
        int jamKeluar = menghitungJamRaw(time(0) - hello);
        int totalJam = jamKeluar - jamMasuk;
        long totalBiaya = 0;
        // perhitungan biaya parkir
        if (kendaraan == parkirMobil) totalBiaya = totalJam * 3000;
        else if (kendaraan == parkirMotor) totalBiaya = totalJam * 2000;
        // mengatur biaya tidak lebih dari 20000
        if (totalBiaya > 20000) totalBiaya = 20000;

        int hari = menghitungHari(time(0) - hello);
        duit[hari] += totalBiaya;
        hapusData(&kendaraan[id]);
      } 
    }

    menampilkanParkiran();
    menampilkanManagement();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  system("clear");
}
