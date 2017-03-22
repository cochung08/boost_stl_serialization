#include <cstddef> // NULL
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/archive/tmpdir.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/assume_abstract.hpp>



class Series_Obj
{
    friend class boost::serialization::access;
    friend std::ostream & operator<<(std::ostream &os, const Series_Obj &gp);


  	int a;
  	int b;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & a;
        ar & b;
    }


  public:
      Series_Obj(){}
      Series_Obj(const int & _a, const int & _b) :
          a(_a), b(_b) {}
      virtual ~Series_Obj(){}
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Series_Obj)

std::ostream & operator<<(std::ostream &os, const Series_Obj &s)
{
    return os << "a is "<< s.a << " b is "<<s.b << ' ' ;
}



class Series_List
{
    friend class boost::serialization::access;
    friend std::ostream & operator<<(std::ostream &os, const Series_List &br);
    typedef Series_Obj * series_obj_pointer;
    std::list<series_obj_pointer> ser_list;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {

        ar & ser_list;
    }
public:
    Series_List(){}
    void append(Series_Obj *_s)
    {
        ser_list.insert(ser_list.end(), _s);
    }
};


std::ostream & operator<<(std::ostream &os, const Series_List &_Series_List)
{
    std::list<Series_Obj*>::const_iterator it;
    for(it = _Series_List.ser_list.begin(); it !=  _Series_List.ser_list.end(); it++){
        os << '\n' <<  ' ' << **it;
            // os << '\n' << *it << ' ' << **it;
    }
    return os;
}


BOOST_CLASS_VERSION(Series_List, 2)

void save_list(const Series_List &s, const char * filename){

    std::cout<<"\nsave_\n";
    std::ofstream ofs(filename);
    boost::archive::text_oarchive oa(ofs);
    oa << s;
}

void
restore_list(Series_List &s, const char * filename)
{
    std::cout <<"\nrestore\n";
    std::ifstream ifs(filename);
    boost::archive::text_iarchive ia(ifs);

    ia >> s;
}

int main(int argc, char *argv[])
{


    Series_Obj *bs0 = new Series_Obj(101,303);
    Series_Obj *bs1 = new Series_Obj(40,88);


    Series_List _Series_List;
    _Series_List.append(bs0);
    _Series_List.append(bs1);


    std::cout << "original Series_List";
    std::cout << _Series_List;

    std::string filename(boost::archive::tmpdir());
    filename += "/demofile123.txt";

    save_list(_Series_List, filename.c_str());


  Series_List new_Series_List;

    restore_list(new_Series_List, filename.c_str());


    std::cout << "\nrestored Series_List";
    std::cout << new_Series_List<<std::endl;


    delete bs0;
    delete bs1;

    return 0;
}
