#include "company.h"
#include "envelope.h"
#include <iostream>
#include <cstring>
#include <sstream>

company_t::company_t(){
    this->name="undefined";
    this->web="undefined";
}
string company_t::get_name() const{
    return this->name;
}
string company_t::get_web() const{
    return this->web;
}
void company_t::handle_e() const{
    for(int i=0;i<this->envelopes_size();i++){
        this->envelopes[i].handle_e();
    }
}
void company_t::handle_i() const{
    cout<<*this<<"\n";
}
void company_t::handle_n()const{
    cout<<this->get_name()<<"\n";
}
void company_t::handle_ec(int postal_code) const{
    for(int i=0;i<this->envelopes_size();i++){
        this->envelopes[i].handle_ec(postal_code);
    }
}
void company_t::handle_ecge(int postal_code) const{
    for(int i=0;i<this->envelopes_size();i++){
        this->envelopes[i].handle_ecge(postal_code);
    }
}
void company_t::handle_ecgt(int postal_code) const{
    for(int i=0;i<this->envelopes_size();i++){
        this->envelopes[i].handle_ecgt(postal_code);
    }
}

void company_t::handle_ecle(int postal_code) const{
    for(int i=0;i<this->envelopes_size();i++){
        this->envelopes[i].handle_ecle(postal_code);
    }
}
void company_t::handle_eclt(int postal_code) const{
    for(int i=0;i<this->envelopes_size();i++){
        this->envelopes[i].handle_eclt(postal_code);
    }
}
void company_t::handle_en(string name ) const{
    for(int i=0;i<this->envelopes_size();i++){
        this->envelopes[i].handle_en(name);
    }
}
void company_t::handle_w() const{
    cout<<this->get_web()<<"\n";
}
envelope_t company_t::envelopes_at(int index) const{
     return this->envelopes.at(index);
}
void company_t::envelopes_push_back(envelope_t envelope){
      this->envelopes.push_back(envelope);
}
int company_t::envelopes_size() const{
   return this->envelopes.size();
 }
void company_t::set_name(string name){
    this->name=name;
}
void company_t::set_web(string web){
    this->web=web;
}
ostream &operator<<(ostream &os, const company_t &company){
     os<< "("<<company.get_name()<<", "<<company.get_web()<<",  (";
    for(int i=0; i< company.envelopes_size()-1;i++){
       os   << company.envelopes_at(i)<<", ";
    }
    os<<company.envelopes_at(company.envelopes_size()-1);
    os  <<"))";

    return os; 
}
