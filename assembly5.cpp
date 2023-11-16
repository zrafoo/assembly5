#include <iostream>
#include <bitset>
#include <vector>
#include <locale.h>

std::vector<unsigned char> invertBits(std::vector<unsigned char> array) 
{
    for (unsigned char& element : array) 
    {
        std::bitset<8> bits(element);

        if ((bits.test(1) == (bits.test(3) && bits.test(5)))&&(bits.test(2) == (bits.test(4) && bits.test(6)))) {
            bits.flip(0);
            bits.flip(7);
        }

        element = static_cast<unsigned char>(bits.to_ulong());
    }

    return array;
}

std::vector<short> shortInvertBits(std::vector<short> array)
{
    for (short& element : array)
    {
        std::bitset<16> bits(element);

        if ((bits.test(1) == (bits.test(3) && bits.test(5))) && (bits.test(2) == (bits.test(4) && bits.test(6)))) {
            bits.flip(0);
            bits.flip(15);
        }

        element = static_cast<short>(bits.to_ulong());
    }

    return array;
}

int main() {

    setlocale(LC_ALL, "rus");

    std::vector<unsigned char> array_c8 = { 0x33, 0x99, 0x5A, 0x77, 0x43, 0x28 };
    std::vector<short> array_c16 = { 0x3312, 0x3310, 0x5A12, 0x7721, 0x43F3, 0x28E1 };
    char array_asm8[6] = { 0x33, 0x99, 0x5A, 0x77, 0x43, 0x28 };
    short array_asm16[6]= { 0x3312, 0x3310, 0x5A12, 0x7721, 0x43F3, 0x28E1 };

    std::vector<unsigned char> result_c8 = invertBits(array_c8);
    std::vector<short> result_c16 = shortInvertBits(array_c16);


    __asm {
        lea esi, array_asm8; // запись в регистр esi начального адреса массива array_asm
        lea edi, array_asm16;
        mov edx, 0; // счетчик
        mov cx, 6; // элементов в массиве
    ONE:
        mov al, [array_asm8 + edx];// загрузить текущий элемент в AL
        mov ah, al; // копируем AL в AH
        mov bl, al; // копируем AL в BL
        shr al, 1; // сдвинуть AL на 1 вправо
        and al, 0x03; //оставить только биты 2:1
        shr ah, 3; //сдвинуть AH на 3 позиции вправо
        and ah, 0x03; //оставить только биты 4:3
        shr bl, 5;//сдвинуть AH на 5 позиций вправо
        and bl, 0x03;//оставить только биты 6:5
        and bl, ah; // 6:5 && 4:3 в BL
        cmp al, bl;
        jnz TWO; //если не равны, перейти к следующему элементу*/
        xor [array_asm8 + edx], 0x81; //инвертировать младший и старший биты (0x81=10000001)
    TWO:
        inc edx;
        dec cx; // убавить счетчик оставшихся слов
        jcxz BIG; // если регистр cx == 0, перейти на BIG
        jmp ONE;
    BIG:
        mov edx, 0; // счетчик
        mov cx, 6; // элементов в массиве
    FOUR:
        mov ax, [array_asm16 + edx];
        mov bx, ax;
        shr bx, 3; 
        and bx, 0x0003; 
        shr ax, 5;
        and ax, 0x0003;
        and bx, ax;
        mov ax, [array_asm16 + edx];
        shr ax, 1;
        and ax, 0x0003;
        cmp ax, bx;
        jnz FIVE; //если не равны, перейти к следующему элементу
        xor [array_asm16 + edx], 0x8001; //инвертировать младший и старший биты (0x8001=100..01)
    FIVE:
        add edx, 2;
        dec cx; // убавить счетчик оставшихся слов
        jcxz END; // если регистр cx == 0, перейти на END
        jmp FOUR;
    END:
        nop;
    }





    std::cout << "Начальный массив: \n";
    for (const auto& element : array_c16) {
        std::cout << std::bitset<16>(element) << " ";
    }
    std::cout << '\n';
    for (const auto& element : array_c16) {
        std::cout << std::hex << static_cast<int>(element) << " ";
    }
    std::cout << "\nРезультат на Си++:\n";
    for (const auto& element : result_c16) {
        std::cout << std::bitset<16>(element) << " ";
    }
    std::cout << '\n';
    for (const auto& element : result_c16) {
        std::cout << std::hex << static_cast<int>(element) << " ";
    }
    std::cout << "\nРезультат на Ассемблере:\n";
    for (const auto& element : array_asm16) {
        std::cout << std::bitset<16>(element) << " ";
    }
    std::cout << '\n';
    for (const auto& element : array_asm16) {
        std::cout << std::hex << static_cast<int>(element) << " ";
    }

    return 0;
}
