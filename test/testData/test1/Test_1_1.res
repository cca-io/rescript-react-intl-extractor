let renderMessages1 = () =>
  <ReactIntl.FormattedMessage id="test1.msg1.1" defaultMessage="This is message 1.1" />

let renderMessages2 = () =>
  <div>
    <FormattedMessage id="test1.msg1.4" defaultMessage="This is message 1.4" />
    <ReactIntl.FormattedMessage id="test1.msg1.5" defaultMessage="This is message 1.5" />
  </div>

let renderMessages3 = () =>
  <div>
    <ReactIntl.FormattedMessage id="test1.msg1.2" defaultMessage="This is message 1.2" />
    <ReactIntl.FormattedMessage
      id="test1.msg1.3"
      defaultMessage="This is message 1.3"
      description="Description for message 1.3"
    />
  </div>

let theseShouldBeIgnored = () =>
  <div>
    <ReactIntl.FormattedMessage x="ignored1.1" y="This message is ignored" />
    <FormattedMessage id="ignored1.2" description="This is a description" />
  </div>

module Msg = {
  open ReactIntl

  @@intl.messages

  let msg18 = {id: "test1.msg1.8", defaultMessage: "This is message 1.8"}

  let ignored1 = {idd: "test1.ignored1.1", defaultMessage: "This message is ignored"}
}

module Msg2 = {
  open ReactIntl

  let ignored2 = {id: "test1.ignored1.2", defaultMessage: "This message is ignored"}
}

module Msg3 = {
  open ReactIntl

  @@intl.messages

  let msg19 =
    @intl.description("Description for message 1.9")
    {id: "test1.msg1.9", defaultMessage: "This is message 1.9"}
}
