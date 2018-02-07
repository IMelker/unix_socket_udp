/*
 * unix_server.h
 *
 *  Created on: 06 февр. 2018 г.
 *      Author: imelker
 */

#ifndef SOURCE_UNIX_SERVER_H_
#define SOURCE_UNIX_SERVER_H_

#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <sys/un.h>
#include <string>

/**
 * @brief Класс сервера слушающего на unix сокете
 * Класс предоставляющий возможность запуска сервера на основе unix сокета
 */
class UnixServer {
 public:
  /**
   * @brief Конструктор
   * @param handler Объект класса агрегирующего метрики
   */
  UnixServer(const std::string& path);
  /**
   * @brief Деструктор
   * Освобождение и закрытие сокета. Освобождение памяти.
   */
  ~UnixServer();

  /**
   * @brief Создает и закрепляет unix сокет
   * Создает unix сокет в формате SOCK_DGRAM и биндит его.
   */
  void Create();

  /**
    * @brief Обслуживание сервера
    * Получает данные с сокета и записывает их в объект класса MetricHandler.
    */
  void Loop();

 private:
  void GetRequest(std::string& request);  // получить данные с сокета

  std::string socket_filename_;  // имя файла
  int server_;  // сокет сервера
  struct sockaddr_un server_addr_;
};

#endif /* SOURCE_UNIX_SERVER_H_ */
