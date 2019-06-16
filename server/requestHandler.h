//
// Created by kugelblitz on 5/25/19.
//

#ifndef INVINCIBLE_SERVER_REQUESTHANDLER_H
#define INVINCIBLE_SERVER_REQUESTHANDLER_H

#include <pistache/endpoint.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>

using namespace Pistache;
using nlohmann::json;

class requestHandler: public Http::Handler {
private:

public:

HTTP_PROTOTYPE(requestHandler);

    void onRequest(const Http::Request& request, Http::ResponseWriter response) {
        std::string datos;
        std::string respuesta = "false";
        if (request.method() == Http::Method::Post) {
            if (request.resource() == "/INSERT") {
                datos = request.body();

                auto jsonRequest = json::parse(datos);
                std::string id = jsonRequest["id"];
                std::string imagen = jsonRequest["imagen"];

                std::cout << "Se recibe json : " << jsonRequest.dump(4) << std::endl;

                /*
                 * Para accesar a parametros del json seria:
                 * var valor = jsonRequest[nomDeLaLlave]
                 */

                std::string temp = jsonRequest["imagen"];
                std::string ID= jsonRequest["id"];
                string ext=jsonRequest["ext"];
                //raidController->receiveImage(ID,temp,ext);
                respuesta = "true";

                // TODO definir respuesta
                response.send(Pistache::Http::Code::Ok, respuesta);
            }

            else if (request.resource() == "/SELECT") {
                datos = request.body();

                auto jsonRequest = json::parse(datos);


                std::string id = jsonRequest["id"];
                std::string ext= jsonRequest["ext"];
                string jsonImagen=raidController->getImage(id+"."+ext);


                /*
                 * Para accesar a parametros del json seria:
                 * var valor = jsonRequest[nomDeLaLlave]
                 */

                // TODO aqui debe estar logica del select

                // TODO definir respuesta
                response.send(Pistache::Http::Code::Ok, jsonImagen);
            }

            else if (request.resource() == "/UPDATE") {
                datos = request.body();

                auto jsonRequest = json::parse(datos);

                /*
                 * Para accesar a parametros del json seria:
                 * var valor = jsonRequest[nomDeLaLlave]
                 */

                // TODO aqui debe estar logica del update

                // TODO definir respuesta
                response.send(Pistache::Http::Code::Ok, jsonRequest.dump(4));
            }

        }
        else {
            std::cout << "se envio respuesta defeault" << std::endl;
            response.send(Pistache::Http::Code::Ok, "<h1>Esta es la respuesta por defecto</h1>");
        }
    }

private:
    RAID_Controller *raidController = new RAID_Controller();
};

#endif //INVINCIBLE_SERVER_REQUESTHANDLER_H
