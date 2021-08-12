/*!
  @header ConnectionStack.h
  @abstract ConnectionStack data structure.
*/

#ifndef CONNECTION_STACK_H
#define CONNECTION_STACK_H

#include <string.h>

/*!
  @abstract Lightweight stack to keep track of connections. This is useful for knowing which connections are active.
*/
class ConnectionStack {
  private:
    /*!
      @abstract ASCII digits 0 through 3 used to track connections as a cheap stack.
      @discussion Most recent connection goes at the end of the string. ESP8266 supports four connections in multiplex mode, which we are using. Therefore, four, plus a null character.
    */
    char m_connections[5];

  public:
    /*!
      @abstract Constructor.
    */
    ConnectionStack();

    /*!
      @abstract Lists connections in the stack.
    */
    char* list();

    /*!
      @abstract Returns the topmost connection in the stack, or NULL if none.
    */
    char* top();

    /*!
      @abstract Adds a connection ID to the top of the stack.
    */
    void add(const char* connectionID);

    /*!
      @abstract Moves a connection ID to the top of the stack.
    */
    void recent(const char* connectionID);

    /*!
      @abstract Removes a connection ID from the stack.
    */
    void remove(const char* connectionID);
};

#endif // !CONNECTION_STACK_H
