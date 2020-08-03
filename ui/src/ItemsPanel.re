open AppState;
open Types;
open Utils;
open Css;

module Styles = {
    let wrapper =
        style([
              5->px->borderRadius,
              300->px->width,
              5->px->padding,
              1->px->border(solid, black),
        ]);

    let inputItem = style([10->px->padding]);

    let ul = style([0->px->padding]);

    let li = style([cursor(`pointer), listStyleType(none)]);
};

type delete =
    | Delete(id, string)
    | NoDelete;

let useDeleteComponents = (~dispatch) => {
    let (showDelete, setShowDelete) = React.useState(_ => NoDelete);
    let triggerDeletion = id =>  setShowDelete(_ => Delete(id, "Are you sure you want to delete?"));
    let onDeleteClick = (id, event) => {
        ReactEvent.Mouse.stopPropagation(event);
        triggerDeletion(id);
    };

    let confirmDeletion = (id, _) => {
        Xhr.delete(
            ~endpoint=Item.endpoint,
            ~onLoad=() => dispatch(DeleteItem(id)),
            ~onError=error => Js.log(error),
            ~id,
        );
    };

    let button = id => <DeleteIcon onClick={onDeleteClick(id)} />;

    let modal =
        fun
            | Delete(id, message) =>
            <DeleteModal
            closeModal={_ => setShowDelete(_ => NoDelete)}
    message
        onConfirmDeletion={confirmDeletion(id)}
    />
        | NoDelete => React.null;

        (button, modal(showDelete), triggerDeletion);
};

let patchItem = (~id, ~onLoad, ~updateItem) => {
    let _ = Xhr.patch(
        ~id,
        ~endpoint=Item.endpoint,
        ~onError=error => Js.log(error),
        ~onLoad=json => onLoad(json->Item.decoder),
        ~data=Js.Json.stringify(Js.Json.object_(updateItem)),
    );
}

module TotalEditor = {
    module Styles = {
        let container = style([
                              display(`flex),
                              alignItems(center),
        ]);
        let total = style([
                          30->px->minWidth,
                          textAlign(center),
        ]);
    }

    [@react.component]
        let make = (~item: Item.t, ~triggerDeletion, ~dispatch) => {
            let onClick = (operation, e) => {
                ReactEvent.Mouse.stopPropagation(e);

                let total = item.total + operation;
                if(total == 0) {
                    triggerDeletion(item.id);
                } else {
                    let updateItem = Js.Dict.empty();
                    Js.Dict.set(updateItem, "total", Js.Json.number(float_of_int(total)));
                    patchItem(
                        ~id=item.id,
                        ~updateItem,
                        ~onLoad=responseItem => dispatch(EditItem(item.id, responseItem)),
                    );
                }
            };

            <div className=Styles.container>
                <MinusIcon onClick=onClick(-1) />
                <span className=Styles.total>item.total->string_of_int->s</span>
                <PlusIcon onClick=onClick(1) />
                </div>
        }
}

[@react.component]
let make = (~state, ~dispatch) => {
    let (inputValue, setInputValue) = React.useState(_ => "");
    let valueCatcher = value => {
        setInputValue(_ => value);
        value;
    };
    let (deleteButton, deleteModal, triggerDeletion) = useDeleteComponents(~dispatch);

    React.useEffect0(() => {
        let abort =
            Xhr.get(
                ~endpoint=Item.endpoint,
                ~onLoad=json => json->Item.array_decoder->LoadItems->dispatch,
                ~onError=error => Js.log(error),
            );
        Some(abort);
    });

    let onClick = id => Utils.DoubleClick.handler(
        selection => dispatch(ToggleItemSelection(selection)),
        id,
    );

    let onNameEdit = (id: AppState.id, name, _resetInput) => {
        let updateItem = Js.Dict.empty();
        Js.Dict.set(updateItem, "name", Js.Json.string(name));
        patchItem(
            ~id,
            ~updateItem,
            ~onLoad=responseItem => dispatch(EditItem(id, responseItem)),
        );
    };

    let onSubmit = (name, resetInput) => {
        let newItem = Js.Dict.empty();
        Js.Dict.set(newItem, "name", Js.Json.string(name));
        Js.Dict.set(newItem, "total", Js.Json.number(1.0));
        let _ = Xhr.post(
            ~endpoint=Item.endpoint,
            ~data=Js.Json.stringify(Js.Json.object_(newItem)),
            ~onLoad=json => {
                json->Item.decoder->AddItem->dispatch;
                resetInput();
            },
            ~onError=error => Js.log(error),
        );
    };

    <>
        deleteModal
        <div className=Styles.wrapper>
        <div className=Styles.inputItem>
        <InputBox onSubmit valueCatcher />
        </div>
        <ul className=Styles.ul>
        {state.items
            ->Belt.Array.keep(item => Js.String.includes(inputValue, item.name))
                ->mapElementArray(item => {
                    let (selected, edit) =
                        switch (state.selectedItem) {
                            | Selected(id) => (id === item.id, false)
                            | Editing(id) => (false, id === item.id)
                            | _ => (false, false)
                            };
                    <li
                        onClick={_ => onClick(item.id)}
                    key={string_of_int(item.id)}
                    className=Styles.li
                        >
                        <Entity
                        onNameEdit
                        edit
                        selected
                        name={item.name}
                    id={item.id}
                    displayBeforeClick={() => <TotalEditor item dispatch triggerDeletion />}
                    displayOnClick=deleteButton
                        />
                        </li>;
                })
        }
    </ul>
        </div>
        </>;
};
