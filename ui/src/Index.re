open AppState;
open Utils;

let initialState: state = {
  entities: [],
  selectedBox: Nothing,
  selectedItem: Nothing,
};

module Styles = {
  open Css;

  let container =
    style([
      10->px->padding,
      900->px->maxWidth,
      margin(auto),
      display(`flex),
      alignItems(stretch),
      justifyContent(spaceEvenly),
    ]);
};

module App = {
  [@react.component]
  let make = _ => {
    let (state, dispatch) = React.useReducer(reducer, initialState);
    <div className=Styles.container>
       <Boxes state dispatch /> </div>;
      /*
       <Items state dispatch />
       */
  };
};

ReactDOMRe.renderToElementWithId(<App />, "root");