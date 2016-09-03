#include "Mesh.h"

Mesh::Mesh()
{

}

template<typename T>
std::string Mesh::toString( const T& value )
{
    std::ostringstream oss;
    try
    {
        // Turn on exceptions
        oss.exceptions( std::ios::badbit | std::ios::failbit );
        oss << value;
    }
    catch ( std::ios_base::failure e )
    {
        std::cerr << e.what() << std::endl;
        // clear the ostream
        oss.clear( std::ios::goodbit );
        return "";
    }
    return oss.str();
}

std::vector<std::string> Mesh::splitString(std::string path, char delimeter)
{
    std::vector<std::string> result;
    std::string current_word = "";

    for(unsigned i = 0; i < path.length(); ++i)
    {
        char current_char = path.at(i);
        if (current_char != delimeter)
        {
            current_word.append( toString(current_char) );
        }

        if ( (current_char == delimeter) or (i == path.length() - 1)) //&& current_word != ""
        {
            result.push_back(current_word);
            current_word = "";
        }
    }

    return result;
}

bool Mesh::loadModel(std::string filename)
{

    ifstream file(filename.c_str());
    if (file.is_open())
    {
        string line = "";

        while( getline(file,line) )
        {
            //cout << "reading" << endl;
            vector<string>  values;
            values = splitString(line,' ');

            if (values.size() == 0) continue;   // skip to next line

            string type = values.at(0);
            if ( type == "v")
            {
                // add the vertex
                v.push_back( Vector3( atof(values.at( 1 ).c_str() ), atof( values.at( 2 ).c_str() ), atof( values.at( 3 ).c_str() ) ) );
            }
            else if  (type == "vt")
            {
                // add vertex texture co-ordinate
                t.push_back( Vector2( atof( values.at( 1 ).c_str() ), atof( values.at( 2 ).c_str() ) ) );
            }
            else if (type == "vn")
            {
                // add vertex normal
                n.push_back( Vector3( atof( values.at( 1 ).c_str() ), atof( values.at( 2 ).c_str() ), atof( values.at( 3 ).c_str() ) ) );
            }
            else if (type == "f")
            {
                // add face data vertex/texture/normal
                Face face;
                for (unsigned i = 1; i < values.size(); ++i)
                {
                    vector<string> d = splitString(values.at(i),'/');
                    int _v = 0;
                    int _t = 0;
                    int _n = 0;

                    if (d.size() > 0) _v = atoi(d.at(0).c_str());
                    if (d.size() > 1) _t = atoi(d.at(1).c_str());
                    if (d.size() > 2) _n = atoi(d.at(2).c_str());

                    Vector3 vd = Vector3(_v,_t,_n);
                    face.vd.push_back(vd);
                }

                f.push_back(face);

            }
        }

        file.close();

        return true;
    }
    else
    {
        cout << "unable to open file: " << filename << endl;
        return false;
    }
}

int Mesh::numVertices()
{
    return v.size();
}

int Mesh::numFaces()
{
    return f.size();
}


void Mesh::DrawModel()
{
    vector<Face>::iterator it = f.begin();

    while ( it != f.end() )
    {
        Face& f = (*it);

        glBegin(GL_POLYGON);

            for (unsigned i = 0; i < f.vd.size(); ++i)
            {
                Vector3 a = f.vd.at(i);
                Vector3 vp = v.at( a.x - 1);
                Vector2 vt = t.at( a.y - 1);
                Vector3 vn = a.z > 0 and a.z < n.size() ? n.at( a.z - 1) : Vector3(0,0,0);

                glNormal3f( vn.x, vn.y, vn.z );
                glTexCoord2f( vt.x, vt.y );
                glVertex3f( vp.x, vp.y, vp.z );
            }

            ++it;
        glEnd();
    }

}
