/*
 * unix_client.h
 *
 *  Created on: 06 февр. 2018 г.
 *      Author: imelker
 */

#ifndef SOURCE_UNIX_CLIENT_H_
#define SOURCE_UNIX_CLIENT_H_

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>

/**
 * @brief Агрегатор информации о состоянии микросервиса
 * Класс предоставляющий возможность агрегировать метрики о состоянии микросервиса и отправлять их к агенту.
 */
class UnixClient {
 public:

  /**
   * @brief Конструктор
   * @param handler Объект класса агрегирующего метрики
   */
  UnixClient(const std::string& path);
  /**
   * @brief Деструктор
   * Освобождение и закрытие сокета. Освобождение памяти.
   */
  ~UnixClient();

  /**
   * @brief Создает и закрепляет unix сокет
   * Создает unix сокет в формате SOCK_DGRAM и биндит его.
   */
  void Connect();

  /**
    * @brief Обслуживание сервера
    * Получает данные с сокета и записывает их в объект класса MetricHandler.
    */
  void Loop();

 private:
  void Send(const std::string& message);

  int client_;
  std::string socket_filename_;
  struct sockaddr_un server_addr_;
};

#endif /* SOURCE_UNIX_CLIENT_H_ */
