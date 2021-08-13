

class ControlParameters
{
  
public:
    void setafn(int afn)
    {
        this->afn = afn;
    }

    void setfn(int fn)
    {
        this->fn = fn;
    }

    void setpn(int pn)
    {
        this->pn = pn;
    }

    void setpfc(int pfc)
    {
        this->pfc = pfc;
    }
    
    int getafn()
    {
        return afn;
    }

    int getfn()
    {
        return fn;
    }

    int getpn()
    {
        return pn;
    }

    int getpfc()
    {
        return pfc;
    }

    void setAdmin_code(unsigned char admincode[])
    {
        this->admin_code[0] = admincode[0];
        this->admin_code[1] = admincode[1];
    }

    void getAdmin_code(unsigned char* admincode)
    {
        admincode[0] = this->admin_code[0];
        admincode[1] = this->admin_code[1];
    }


    void setconcentrate_no(unsigned char concentrateno[])
    {
        this->concentrate_no[0] = concentrateno[0];
        this->concentrate_no[1] = concentrateno[1];
    }

    void getconcentrate_no(unsigned char* concentrateno[])
    {
        concentrateno[0] = concentrate_no[0];
        concentrateno[1] = concentrate_no[1];
    }

    void setMSA(unsigned char msa)
    {
        this->MSA =  msa;
    }

    unsigned char getMSA()
    {

        return MSA;
    }


private:
    int afn;
    int fn;
    int pn;
    int pfc;
    unsigned int admin_code[2];
    unsigned char concentrate_no[2];
    unsigned char MSA; 

};
