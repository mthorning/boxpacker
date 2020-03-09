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
let make = (~placeholder="", ~onSubmit) => {
  let (value, setValue) = React.useState(_ => "");

  let onChange = event => {
    let inputVal = event->ReactEvent.Form.target##value;
    setValue(_ => inputVal);
  };

  let onKeyDown = event =>
    if (event->ReactEvent.Keyboard.keyCode === 13 && value !== "") {
      onSubmit(value, () => setValue(_ => ""));
    };

  let stop = event => ReactEvent.Mouse.stopPropagation(event);
  let onBlur = _ => Js.log("blur");

  <div onClick=stop className=Styles.container>
    <input type_="search" placeholder onChange onKeyDown value onBlur />
  </div>;
};