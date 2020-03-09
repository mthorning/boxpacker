open AppState;
open Utils;

module Styles = {
  open Css;

  let container =
    style([
      display(`flex),
      flexDirection(column),
      100.00->`percent->height,
    ]);
  let formItem =
    style([
      display(`flex),
      justifyContent(spaceBetween),
      100.00->`percent->width,
      5->px->marginTop,
    ]);
  let label = style([textAlign(`right), 5->px->marginRight]);
};

module FormItem = {
  [@react.component]
  let make = (~children, ~label) => {
    <div className=Styles.formItem>
      <span className=Styles.label> label </span>
      children
    </div>;
  };
};

[@react.component]
let make = (~closeModal, ~getItem) => {
  let item: entity = getItem();
  <Modal closeModal>
    <div className=Styles.container>
      <h2> item.name->s </h2>
      <FormItem label={"Quantity"->s}> <input type_="number" /> </FormItem>
      <FormItem label={"Description"->s}> <textarea /> </FormItem>
    </div>
  </Modal>;
};