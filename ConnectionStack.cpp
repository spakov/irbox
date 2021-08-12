#include "ConnectionStack.h"

ConnectionStack::ConnectionStack() {
  memset(m_connections, 0, 5);
}

char* ConnectionStack::list() {
  return m_connections;
}

char* ConnectionStack::top() {
  if (!strlen(m_connections)) return NULL;
  return m_connections + strlen(m_connections) - (size_t) 1;
}

void ConnectionStack::add(const char* connectionID) {
  if (strstr(m_connections, connectionID) == NULL)
    strcat(m_connections, connectionID);
}

void ConnectionStack::recent(const char* connectionID) {
  ConnectionStack::remove(connectionID);
  ConnectionStack::add(connectionID);
}

void ConnectionStack::remove(const char* connectionID) {
  static char temp[5];

  memset(temp, 0, 5);
  for (size_t i = 0; i < 5; i++) {
    if (i < strlen(m_connections) && m_connections[i] != connectionID[0]) {
      temp[strlen(temp)] = m_connections[i];
    }
  }
  strcpy(m_connections, temp);
}
