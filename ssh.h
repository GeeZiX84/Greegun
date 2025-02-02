#pragma once
class Connect
{
  public:
    Connect(const char *i,const char *u,const char *p);
    void file();
  private:
    const char *ip_adress;
    const char *username;
    const char *password;
};