open Utils;

module InputStyles = {
  open Css;

  let container = style([15->px->padding, position(relative)]);
  let input =
    style([
      5->px->borderRadius,
      30->px->height,
      100.00->`percent->width,
      boxSizing(borderBox),
    ]);
  let closeIcon = visible =>
    style([
      20->px->top,
      20->px->right,
      20->px->fontSize,
      position(absolute),
      cursor(`pointer),
      display(visible ? block : none),
    ]);
};

module Styles = {
  open Css;

  let container = selected => {
    let base = [2->px->padding, 1->px->borderBottom(solid, black)];
    style(selected ? Theme.selectionColor @ base : base);
  };
  let entity =
    style([
      display(`flex),
      justifyContent(spaceBetween),
      alignItems(center),
    ]);
};

[@react.component]
let make =
    (~edit, ~selected, ~onEdit as onSubmit, ~name, ~id, ~displayOnEntityClick) => {
  <div className={Styles.container(selected)}>
    {edit
       ? <InputBox onSubmit={onSubmit(id)} />
       : <div className=Styles.entity>
           name->s
           {selected ? displayOnEntityClick(id) : React.null}
         </div>}
  </div>;
};