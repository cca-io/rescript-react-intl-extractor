let _ =
  @intl.messages
  {
    "test4.msg2.1": {
      "id": "test4.msg2.1",
      "defaultMessage": "This is message 2.1",
    },
    "test4.msg2.2": {
      "id": "test4.msg2.2",
      "defaultMessage": "This is message 2.2",
      "description": "Description for message 2.2",
    },
    "test4.ignored2.1": {
      "idd": "ignored2.1" /* This will not be extracted. */,
      "defaultMessage": "This is message 13",
    },
  }

let _ =
  @intl
  {
    "ignored2.2": {
      "id": "ignored2.2",
      "defaultMessage": "This message is ignored",
    },
    "ignored2.3": {
      "id": "ignored2.3",
      "defaultMessage": "This message is ignored, too",
    },
  }
