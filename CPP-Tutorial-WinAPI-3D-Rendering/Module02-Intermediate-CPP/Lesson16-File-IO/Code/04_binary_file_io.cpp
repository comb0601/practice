#include <iostream>
#include <fstream>
using namespace std;
int main() {
    int numbers[] = {1, 2, 3, 4, 5};
    ofstream out("binary.dat", ios::binary);
    out.write(reinterpret_cast<char*>(numbers), sizeof(numbers));
    out.close();
    int read_numbers[5];
    ifstream in("binary.dat", ios::binary);
    in.read(reinterpret_cast<char*>(read_numbers), sizeof(read_numbers));
    in.close();
    cout << "Read: ";
    for(int n : read_numbers) cout << n << " ";
    cout << endl;
    return 0;
}
