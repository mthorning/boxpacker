open AppState;

let initialState: state = {
    containers: [||],
    items: [||],
    selectedContainer: Nothing,
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
            <div className=Styles.container> <ItemsPanel state dispatch /> </div>;
            /*
               <Items state dispatch />
             */
        };
};

ReactDOMRe.renderToElementWithId(<App />, "root");
