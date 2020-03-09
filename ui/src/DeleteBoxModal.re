open Utils;

module Styles = {
  open Css;

  let container = style([display(`flex), flexDirection(column)]);
  let buttons = style([textAlign(`right)]);
  let button = style([5->px->marginLeft, cursor(`pointer)]);
};

[@react.component]
let make = (~onConfirmDeletion, ~closeModal, ~message) => {
  let onDelete = _ => {
    onConfirmDeletion();
    closeModal();
  };

  <Modal closeModal>
    <div className=Styles.container>
      <p> message->s </p>
      <div className=Styles.buttons>
        <button className=Styles.button onClick={_ => closeModal()}>
          "No"->s
        </button>
        <button className=Styles.button onClick=onDelete> "Yes"->s </button>
      </div>
    </div>
  </Modal>;
};