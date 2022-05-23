class numbers
{
public:
    void printNumber(int x, int num)
    {
        switch (num)
        {
        case 0:
            numbers::print0(x);
            break;
        case 1:
            numbers::print1(x);
            break;
        case 2:
            numbers::print2(x);
            break;
        case 3:
            numbers::print3(x);
            break;
        case 4:
            numbers::print4(x);
            break;
        case 5:
            numbers::print5(x);
            break;
        case 6:
            numbers::print6(x);
            break;
        case 7:
            numbers::print7(x);
            break;
        case 8:
            numbers::print8(x);
            break;
        case 9:
            numbers::print9(x);
            break;
        }
    }
    void dot(int x, bool on)
    {
        if (on)
        {
            printdot(x);
        }
        if (!on)
        {
            clrdot(x);
        }
    }
    int convert(int nmb, int pos)
    {
        switch (pos)
        {
        case 1:
            return nmb / 10;
            break;
        case 2:
            return nmb % 10;
            break;
        }
    }

private:
    void printdot(int x)
    {
        lcd.setCursor(x, 0);
        lcd.write(BLOCKDOWN);
        lcd.setCursor(x, 1);
        lcd.write(BLOCKUP);
        lcd.setCursor(x, 2);
        lcd.write(BLOCKDOWN);
        lcd.setCursor(x, 3);
        lcd.write(BLOCKUP);
    }
    void clrdot(int x)
    {
        lcd.setCursor(x, 0);
        lcd.print(" ");
        lcd.setCursor(x, 1);
        lcd.print(" ");
        lcd.setCursor(x, 2);
        lcd.print(" ");
        lcd.setCursor(x, 3);
        lcd.print(" ");
    }
    void print0(int x)
    {
        lcd.setCursor(x, 0);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 1, 0);
        lcd.write(BLOCK);
        lcd.setCursor(x + 2, 0);
        lcd.write(LEFTDOWN);
        lcd.setCursor(x, 1);
        lcd.write(BLOCK);
        lcd.setCursor(x, 2);
        lcd.write(BLOCK);
        lcd.setCursor(x + 1, 1);
        lcd.print(" ");
        lcd.setCursor(x + 1, 2);
        lcd.print(" ");
        lcd.setCursor(x + 2, 1);
        lcd.write(BLOCK);
        lcd.setCursor(x + 2, 2);
        lcd.write(BLOCK);
        lcd.setCursor(x, 3);
        lcd.write(RIGHTUP);
        lcd.setCursor(x + 1, 3);
        lcd.write(BLOCK);
        lcd.setCursor(x + 2, 3);
        lcd.write(LEFTUP);
    }
    void print1(int x)
    {
        lcd.setCursor(x, 0);
        lcd.print(" ");
        lcd.setCursor(x + 1, 0);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 2, 0);
        lcd.write(BLOCK);
        lcd.setCursor(x, 1);
        lcd.print(" ");
        lcd.setCursor(x + 1, 1);
        lcd.print(" ");
        lcd.setCursor(x + 2, 1);
        lcd.write(BLOCK);
        lcd.setCursor(x, 2);
        lcd.print(" ");
        lcd.setCursor(x + 1, 2);
        lcd.print(" ");
        lcd.setCursor(x + 2, 2);
        lcd.write(BLOCK);
        lcd.setCursor(x, 3);
        lcd.print(" ");
        lcd.setCursor(x + 1, 3);
        lcd.print(" ");
        lcd.setCursor(x + 2, 3);
        lcd.write(BLOCK);
    }
    void print2(int x)
    {
        lcd.setCursor(x, 0);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 1, 0);
        lcd.write(BLOCK);
        lcd.setCursor(x + 2, 0);
        lcd.write(LEFTDOWN);
        lcd.setCursor(x, 1);
        lcd.print(" ");
        lcd.setCursor(x + 1, 1);
        lcd.print(" ");
        lcd.setCursor(x + 2, 1);
        lcd.write(BLOCK);
        lcd.setCursor(x, 2);
        lcd.print(" ");
        lcd.setCursor(x + 1, 2);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 2, 2);
        lcd.write(LEFTUP);
        lcd.setCursor(x, 3);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 1, 3);
        lcd.write(BLOCK);
        lcd.setCursor(x + 2, 3);
        lcd.write(BLOCK);
    }
    void print3(int x)
    {
        lcd.setCursor(x, 0);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 1, 0);
        lcd.write(BLOCK);
        lcd.setCursor(x + 2, 0);
        lcd.write(LEFTDOWN);
        lcd.setCursor(x, 1);
        lcd.print(" ");
        lcd.setCursor(x + 1, 1);
        lcd.write(BLOCKDOWN);
        lcd.setCursor(x + 2, 1);
        lcd.write(LEFTUP);
        lcd.setCursor(x, 2);
        lcd.print(" ");
        lcd.setCursor(x + 1, 2);
        lcd.write(BLOCKUP);
        lcd.setCursor(x + 2, 2);
        lcd.write(LEFTDOWN);
        lcd.setCursor(x, 3);
        lcd.write(RIGHTUP);
        lcd.setCursor(x + 1, 3);
        lcd.write(BLOCK);
        lcd.setCursor(x + 2, 3);
        lcd.write(LEFTUP);
    }
    void print4(int x)
    {
        lcd.setCursor(x, 0);
        lcd.print(" ");
        lcd.setCursor(x + 1, 0);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 2, 0);
        lcd.write(BLOCK);
        lcd.setCursor(x, 1);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 1, 1);
        lcd.write(LEFTUP);
        lcd.setCursor(x + 2, 1);
        lcd.write(BLOCK);
        lcd.setCursor(x, 2);
        lcd.write(BLOCK);
        lcd.setCursor(x + 1, 2);
        lcd.write(BLOCK);
        lcd.setCursor(x + 2, 2);
        lcd.write(BLOCK);
        lcd.setCursor(x, 3);
        lcd.print(" ");
        lcd.setCursor(x + 1, 3);
        lcd.print(" ");
        lcd.setCursor(x + 2, 3);
        lcd.write(BLOCK);
    }
    void print5(int x)
    {
        lcd.setCursor(x, 0);
        lcd.write(BLOCK);
        lcd.setCursor(x + 1, 0);
        lcd.write(BLOCKUP);
        lcd.setCursor(x + 2, 0);
        lcd.write(BLOCKUP);
        lcd.setCursor(x, 1);
        lcd.write(RIGHTUP);
        lcd.setCursor(x + 1, 1);
        lcd.write(LEFTDOWN);
        lcd.setCursor(x + 2, 1);
        lcd.print(" ");
        lcd.setCursor(x, 2);
        lcd.print(" ");
        lcd.setCursor(x + 1, 2);
        lcd.write(RIGHTUP);
        lcd.setCursor(x + 2, 2);
        lcd.write(LEFTDOWN);
        lcd.setCursor(x, 3);
        lcd.write(RIGHTUP);
        lcd.setCursor(x + 1, 3);
        lcd.write(BLOCKDOWN);
        lcd.setCursor(x + 2, 3);
        lcd.write(LEFTUP);
    }
    void print6(int x)
    {
        lcd.setCursor(x, 0);
        lcd.print(" ");
        lcd.setCursor(x + 1, 0);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 2, 0);
        lcd.write(LEFTUP);
        lcd.setCursor(x, 1);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 1, 1);
        lcd.write(BLOCK);
        lcd.setCursor(x + 2, 1);
        lcd.print(" ");
        lcd.setCursor(x, 2);
        lcd.write(BLOCK);
        lcd.setCursor(x + 1, 2);
        lcd.print(" ");
        lcd.setCursor(x + 2, 2);
        lcd.write(LEFTDOWN);
        lcd.setCursor(x, 3);
        lcd.write(RIGHTUP);
        lcd.setCursor(x + 1, 3);
        lcd.write(BLOCK);
        lcd.setCursor(x + 2, 3);
        lcd.write(LEFTUP);
    }
    void print7(int x)
    {
        lcd.setCursor(x, 0);
        lcd.write(BLOCKUP);
        lcd.setCursor(x + 1, 0);
        lcd.write(BLOCKUP);
        lcd.setCursor(x + 2, 0);
        lcd.write(BLOCK);
        lcd.setCursor(x, 1);
        lcd.print(" ");
        lcd.setCursor(x + 1, 1);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 2, 1);
        lcd.write(LEFTUP);
        lcd.setCursor(x, 2);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 1, 2);
        lcd.write(LEFTUP);
        lcd.setCursor(x + 2, 2);
        lcd.print(" ");
        lcd.setCursor(x, 3);
        lcd.write(BLOCK);
        lcd.setCursor(x + 1, 3);
        lcd.print(" ");
        lcd.setCursor(x + 2, 3);
        lcd.print(" ");
    }
    void print8(int x)
    {
        lcd.setCursor(x, 0);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 1, 0);
        lcd.write(BLOCKUP);
        lcd.setCursor(x + 2, 0);
        lcd.write(LEFTDOWN);
        lcd.setCursor(x, 1);
        lcd.write(RIGHTUP);
        lcd.setCursor(x + 1, 1);
        lcd.write(BLOCKDOWN);
        lcd.setCursor(x + 2, 1);
        lcd.write(LEFTUP);
        lcd.setCursor(x, 2);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 1, 2);
        lcd.write(BLOCKUP);
        lcd.setCursor(x + 2, 2);
        lcd.write(LEFTDOWN);
        lcd.setCursor(x, 3);
        lcd.write(RIGHTUP);
        lcd.setCursor(x + 1, 3);
        lcd.write(BLOCKDOWN);
        lcd.setCursor(x + 2, 3);
        lcd.write(LEFTUP);
    }
    void print9(int x)
    {
        lcd.setCursor(x, 0);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 1, 0);
        lcd.write(BLOCK);
        lcd.setCursor(x + 2, 0);
        lcd.write(LEFTDOWN);
        lcd.setCursor(x + 2, 1);
        lcd.write(BLOCK);
        lcd.setCursor(x + 1, 1);
        lcd.print(" ");
        lcd.setCursor(x, 1);
        lcd.write(RIGHTUP);
        lcd.setCursor(x, 2);
        lcd.print(" ");
        lcd.setCursor(x + 1, 2);
        lcd.write(BLOCK);
        lcd.setCursor(x + 2, 2);
        lcd.write(LEFTUP);
        lcd.setCursor(x, 3);
        lcd.write(RIGHTDOWN);
        lcd.setCursor(x + 1, 3);
        lcd.write(LEFTUP);
        lcd.setCursor(x + 2, 3);
        lcd.print(" ");
    }
};

extern numbers nmb;