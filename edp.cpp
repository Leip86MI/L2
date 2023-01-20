#include "address.h"
#include "envelope.h"
#include "company.h"
#include "envelope-dl.h"
#include "envelope-c4.h"
#include "str2l-error.h"
#include "str2i-error.h"
#include <cstring> /*  for string */
#include <cstdlib>
#include "pugixml.hpp" /* for pugixml */
#include <iostream> /*  for cout */
#include <climits>
#include <cstdio>

using namespace std;
using namespace pugi;


void instruction_h(){
	cout << "e: prints the envelopes" <<endl;
	cout << "ec CODE: prints the envelopes with the sender postal code equal to CODE" << endl;
	cout << "ecge CODE: prints the envelopes with the sender postal code greater than or equal to CODE" << endl;
	cout << "ecgt CODE: prints the envelopes with the sender postal code greater than CODE" << endl;
	cout << "ecle CODE: prints the envelopes with the sender postal code less than or equal to CODE" << endl;
	cout << "eclt CODE: prints the envelopes with the sender postal code less than CODE" << endl;
	cout << "en NAME: prints the envelopes with the sender name containing NAME" << endl;
	cout << "h: prints this help" << endl;
	cout << "i: prints information about the company" << endl;
	cout << "n: prints the company name" << endl;
	cout << "q: quits EDP" << endl;
	cout << "v: prints the EDP version" << endl;
	cout << "w: prints the company web address" << endl;
	}

void instruction_v(){
	cout << "EDP (Envelope Delivery Program) 2"<<'\n'<< endl;
	cout << "Copyright (C) 2022 Lucas Martin de fourchambault and Yann Simo Junior Tatchim."<<"\n" <<endl;
	cout << "Written by Lucas Martin de fourchambault <lucas.martin-de-fourchamdault@univ-pau.fr> and Yann Simo Junior Tatchim <yann-simo.junior-tatchim@univ-pau.fr>."<<"\n" << endl;
	}

void fullfill(xml_node node, envelope_t * envelope,address_t address,const char *format,const char *typeenvoie){
	long int postal_code;
	char *endptr=NULL;
	if(strcmp(node.first_attribute().value(),format)==0){
		for(node=node.first_child();node;node=node.next_sibling()){
			if(strcmp(node.name(),"priority")==0){
				if(strcmp(node.child_value(),"low")==0){
					envelope->set_priority(low);
					
					}
				if(strcmp(node.child_value(),"high")==0){
					envelope->set_priority(high);
					}
				if(strcmp(node.child_value(),"medium")==0){
					envelope->set_priority(medium);
					}
				if(strcmp(node.child_value(),"undefined")==0){
					envelope->set_priority(undefined);
					}
				}
			if(strcmp(node.name(),typeenvoie)==0){
				for(node=node.first_child();node;node=node.next_sibling()){
					if(strcmp(node.name(),"city")==0){
						
						address.set_city(node.child_value());
						}
					if(strcmp(node.name(),"country")==0){
						address.set_country(node.child_value());
						}
					if(strcmp(node.name(),"name")==0){
						address.set_name(node.child_value());
						}
					if(strcmp(node.name(),"postal-code")==0){
						postal_code=strtol(node.child_value(),&endptr,10);
                        cout << endptr;
                        if(*endptr!='\0'){
                            throw str2l_error(node.child_value());
                        }
						
                        else if(postal_code >INT_MAX && postal_code<LONG_MAX ){
                            
                            throw str2i_error(node.child_value());
                        }
                        else if(postal_code >INT_MAX)
                        {
                            throw str2l_error(node.child_value()); 
                        }
                        

						address.set_postal_code(postal_code);
						}
					if(strcmp(node.name(),"street")==0){
						address.set_street(node.child_value());
						}
					}
				}
			}
		if(strcmp(typeenvoie,"sender")==0){
			envelope->set_sender(address);
			}
		if(strcmp(typeenvoie,"recipient")==0)
		{
			envelope->set_recipient(address);
		}
		}
	}
int main(int argc, char ** argv){
    char *buf = new char[20];
	int  j = 0;
	int cpt = 0;
	char * ptr;
	char * ptr2;
	long ret = 0;
	char * out = (char*)"q";
	char func[30];
	xml_document doc;
	xml_node node;
	envelope_dl_t *envelopedl = new envelope_dl_t();
	envelope_c4_t *envelopec4 = new envelope_c4_t();
	company_t *company = new company_t();
	address_t recipientaddressc4;
	address_t recipeintaddressdl;
	address_t senderaddressc4;
	address_t senderaddressdl;
	xml_parse_result result;
	result = doc.load_file(argv[1]);
	if ( argc != 2){
        cerr << argv[0] << ": invalid number of arguments" <<endl;
        return 1;
    }
	if( !result ){
		cerr << argv[0] << ": unable to parse the document" <<endl;
		return 1;
		}
	node = doc.first_child();
	if(strcmp(node.name(),"company")==0){
		company->set_name(node.first_attribute().value());
		for(node=node.first_child();node;node=node.next_sibling()){
			if(strcmp(node.name(),"envelopes")==0){
				for(node=node.first_child();node;node=node.next_sibling()){
					if(strcmp(node.name(),"envelope")==0){
                       try{
						fullfill(node,envelopec4,recipientaddressc4, "c4","recipient");
                       }
                       catch(str2l_error &e){
                           cerr<<argv[0]<<" an exception occured: "<<e.what()<<endl;
                           return 1;
                       }
                       catch(str2i_error &e){
                           cerr<<argv[0]<<" an exception occured: "<<e.what()<<endl;
                           return 1;
                       }
                       try{
						fullfill(node,envelopec4,senderaddressc4, "c4","sender");
                       }
                       catch(str2i_error &e){
                           cerr<<argv[0]<<" an exception occured: "<<e.what()<<endl;
                           return 1;
                       }
                       catch(str2l_error &e){
                           cerr<<argv[0]<<" an exception occured: "<<e.what()<<endl;
                           return 1;
                       }
                       try{
						fullfill(node,envelopedl,recipeintaddressdl, "dl","recipient");
                       }
                       catch(str2i_error &e){
                           cerr<<argv[0]<<" an exception occured: "<<e.what()<<endl;
                           return 1;
                       }
                       catch(str2l_error &e){
                           cerr<<argv[0]<<" an exception occured: "<<e.what()<<endl;
                           return 1;
                       }
                       try{
						 
						fullfill(node,envelopedl,senderaddressdl, "dl","sender");
                       }
                       catch(str2l_error &e){
                           cerr<<argv[0]<<" an exception occured: "<<e.what()<<endl;
                           return 1;
                       }
                        catch(str2i_error &e){
                           cerr<<argv[0]<<" an exception occured: "<<e.what()<<endl;
                           return 1;
                       }

					}
                }
            }
         }
    }
	company->envelopes_push_back(*envelopec4);
	company->envelopes_push_back(*envelopedl);
	node = doc.first_child();
	if(strcmp(node.name(),"company")==0){
		for(node=node.first_child();node;node=node.next_sibling()){
			if(strcmp(node.name(),"web")==0){
				company->set_web(node.child_value());
				}
			}
		}
	do
	{
		cout << "EDP> ";
		fgets(func,100,stdin);
		if ( strlen ( func) > 18){
			cerr << "./edp.out: too many characters for the command" <<endl;
            return 1;
			}
		else if ( strlen(func) < 3 ){
			if ( strcmp(func, "e\n")==0){
				company->handle_e();
				}
			else if ( strcmp(func,"h\n")==0){
				instruction_h();
				}
			else if ( strcmp(func,"i\n")==0){
				company->handle_i();
				}
			else if ( strcmp(func,"n\n")==0){
				company->handle_n();
				}
			else if ( strcmp(func,"w\n")==0){
				company->handle_w();
				}
			else if ( strcmp(func,"v\n")==0){
				instruction_v();
				}
			else if ( strcmp(func,"q\n") !=0  ){
				cerr << "./edp.out: invalid command" <<endl;;

				}
			}
		else if ( strstr(func , (char *) "eclt") != NULL){
			if ( strlen(func) == 5){
				cerr << "./edp.out: missing parameter for the eclt command"<<endl;
				}
			else {
				ptr = &func[5];
				ret = strtol(ptr , &ptr2, 10);
				if ( !*ptr2 || ret ==0 || ret > INT_MAX ){
					cerr << "./edp.out: invalid parameter for the eclt command" << endl;
					}
				else{
					company->handle_eclt(ret);
					}
				}
			}
		else if ( strstr(func , (char *) "ecle") != NULL){
			if ( strlen(func) == 5){
				cerr << "./edp.out: missing parameter for the ecle command"<<endl;
				}
			else {
				ptr = &func[5];
				ret = strtol(ptr , &ptr2, 10);
				if ( !*ptr2 || ret ==0 || ret > INT_MAX ){
					cerr << "./edp.out: invalid parameter for the ecle command" << endl;
					}
				else{
					company->handle_ecle(ret);
					}
				}
			}
		else if ( strstr(func , (char *) "ecgt") != NULL){
			if ( strlen(func) == 5){
				cerr << "./edp.out: missing parameter for the ecgt command"<<endl;
				}
			else {
				ptr = &func[5];
				ret = strtol(ptr , &ptr2, 10);
				if ( !*ptr2 || ret ==0 || ret > INT_MAX ){
					cerr << "./edp.out: invalid parameter for the ecgt command" << endl;
					}
				else{
					company->handle_ecgt(ret);
					}
				}
			}
		else if ( strstr(func , (char *) "ecge") != NULL){
			if ( strlen(func) == 5){
				cerr << "./edp.out: missing parameter for the ecge command"<<endl;
				}
			else {
				ptr = &func[5];
				ret = strtol(ptr , &ptr2, 10);
				if ( !*ptr2 || ret ==0 || ret > INT_MAX ){
					cerr << "./edp.out: invalid parameter for the ecge command" << endl;
					}
				else{
					company->handle_ecge(ret);
					}
				}
			}
		else if ( strstr(func , (char *) "ec") != NULL){
			if ( strlen(func) == 3){
				cerr << "./edp.out: missing parameter for the ec command"<<endl;
				}
			else {
				ptr = &func[3]; /* pointe sur para */
				ret = strtol(ptr , &ptr2, 10);
				if (!*ptr2 || ret ==0 || ret > INT_MAX  ){
					cerr << "./edp.out: invalid parameter for the ec command" << endl;
					}
				else{
					company->handle_ec(ret);
					}
				}
			
			}
		else if ( strstr(func , (char *) "en") != NULL){
			if ( strlen(func) == 3){
				cerr << "./edp.out: missing parameter for the en command"<<endl;
				}
			else {
				ptr = &func[3];
				ret = strtol(ptr , &ptr2, 10);
				if ( ret == 0){
					while(j<static_cast<int>(strlen(ptr)-1)){
						buf[cpt] = ptr[j];
						cpt++;
						j++;
						}
					while(buf[cpt]!='\0'){
						buf[cpt]='\0';
						cpt++;
						}
					company->handle_en(buf);
					}
				else 
                {
					cerr << "./edp.out: invalid parameter for the en command"<<endl;
				}
				}
			}
		else {
			cerr << "./edp.out: invalid command" <<endl;
			}
		}while ( (out[0] != func[0] ) || (strlen(func) != 2));
	delete envelopedl;
	delete envelopec4;
	delete company;
    delete buf;
	return 0;
}