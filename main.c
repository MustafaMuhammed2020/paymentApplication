#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include"terminal\terminal.h"
#include"server\server.h"
#include"Application\app.h"

int main()
{
	getDataBase(accountsDB);
	appStart();
}