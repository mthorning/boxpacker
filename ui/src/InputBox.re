module Styles = {
  open Css;
  let container = style([position(relative)]);
  let closeIcon = visible =>
    style([
      5->px->top,
      5->px->right,
      20->px->fontSize,
      position(absolute),
      cursor(`pointer),
      display(visible ? block : none),
    ]);
};

[@react.component]
let make = (~valueCatcher=value => value, ~placeholder="", ~onSubmit) => {
  let (value, setValue) = React.useState(_ => "");

  let onChange = event => {
    let inputVal = event->ReactEvent.Form.target##value;
    setValue(_ => valueCatcher(inputVal));
  };

  let onKeyDown = event =>
    if (event->ReactEvent.Keyboard.keyCode === 13 && value !== "") {
      onSubmit(value, () => setValue(_ => valueCatcher("")));
    };

  let stop = event => ReactEvent.Mouse.stopPropagation(event);

  <div onClick=stop className=Styles.container>
    <input autoFocus=true type_="search" placeholder onChange onKeyDown value />
  </div>;
};
