module Styles = {
  open Css;

  let center =
    style([display(`flex), alignItems(center), justifyContent(center)]);
  let container =
    merge([
      style([
        position(fixed),
        0->px->top,
        0->px->right,
        0->px->bottom,
        0->px->left,
        5->zIndex,
        rgba(0, 0, 0, 0.7)->background,
      ]),
      center,
    ]);

  let modal =
    merge([
      style([
        5->px->borderRadius,
        50.00->vh->width,
        700->px->maxWidth,
        200->px->minHeight,
        400->px->minWidth,
        50.00->vw->width,
        background(white),
      ]),
      center,
    ]);
};

[@react.component]
let make = (~children, ~closeModal) => {
  let onContainerClick = event => {
    ReactEvent.Mouse.stopPropagation(event);
    closeModal();
  };
  let onModalClick = event => {
    ReactEvent.Mouse.stopPropagation(event);
  };
  <div className=Styles.container onClick=onContainerClick>
    <div className=Styles.modal onClick=onModalClick> children </div>
  </div>;
};