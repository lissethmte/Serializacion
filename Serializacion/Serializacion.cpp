#include <iostream>
#include <fstream>
#include <conio.h>
#include <json/json.h>
#include <curl/curl.h>

void writeHandler(void* data, size_t size, size_t nmemb, void* stream)
{
    //std::cout << "writehandler \n";

    //guardar fact a disco
    //std::cout << (char*)data << "/n";
    Json::Value res;
    std::stringstream ss;
    ss.write((const char*)data, nmemb);
    ss >> res;

    std::cout << res["fact"] << "\n";
}

//Callback llamado al retornar la peticion de 1 a 10 factoides
void factsHandler(void* data, size_t size, size_t nmemb, void* stream)
{
    std::cout << "factsHandler \n";
    Json::Value res;
    std::stringstream ss;
    ss.write((const char*)data, nmemb);
    ss >> res;
    int n = res["to"].asInt();
    std::cout << "to = " << n << "\n";
    //imprimir los factoides en forma de lista
    for (int i = 0; i < n; i++)
    {
        std::cout << res["data"][i]["fact"] << "\n";
    }
}

int main()
{
    //declarar un nodo json
    Json::Value root;
    std::cout << "Hello World!\n";
    std::ifstream datafile("data.json", std::ifstream::binary);
    if (datafile.is_open())
    {
        try
        {
            datafile >> root;
            std::cout << "json nulo? " << root.isNull() << "\n";
            std::cout << root["llave"] << std::endl;
            std::cout << root["key"] << std::endl;
            std::cout << root["myArray"] << std::endl;
            std::cout << root["myObject"] << std::endl;

            root["employee"] = Json::objectValue;
            root["employee"]["name"] = "Hugfo";
            root["employee"]["times"] = Json::arrayValue;
            for (int i = 0; i < 10; i++)
            {
                root["employee"]["times"].append(rand() * 10);
            }
            std::cout << root << "\n";

            //escribir json modificado a disco
            std::ofstream newfile("data1.json");
            newfile << root;
            newfile.close();
        }
        catch (const std::exception& e)
        {
            std::cout << "Error en json: ";
            std::cout << e.what() << "\n";
        }

    }
    else
    {
        std::cout << "json no encontrado\n";
    }

    std::cout << "¿Cuantos datos de gatos deseas? (1 a 10)" << std::endl;
    int n = 0;
    std::cin >> n;

    std::cout << "ingresado: " << n << "\n";
    if (n > 0 && n <= 10)
    {
        CURL* curlobj = curl_easy_init();
        if (curlobj)
        {
            if (n == 1)
            {
                curl_easy_setopt(curlobj, CURLOPT_URL, "https://catfact.ninja/fact");
            }
            else
            {
                std::string url = std::string("https://catfact.ninja/facts?limit=");
                url += std::to_string(n);
                curl_easy_setopt(curlobj, CURLOPT_URL, url.c_str());
            }

            curl_easy_setopt(curlobj, CURLOPT_WRITEFUNCTION, factsHandler);
            curl_easy_perform(curlobj);
            curl_easy_cleanup(curlobj);
        }
    }
    else
    {
        std::cout << "numero incorrecto" << "\n";
    }

    return 0;

}



