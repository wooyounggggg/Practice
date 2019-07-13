import React, { Component } from 'react';
import TOC from "./components/TOC";
import ReadContent from "./components/ReadContent";
import CreateContent from "./components/CreateContent";
import Subject from "./components/Subject";
import Control from "./components/Control";
import './App.css';


class App extends Component {
	constructor(props){
		super(props);
		this.state = {
			mode:'welcome',
			title:'HTML',
			subject:{title:"WEB", sub:"World Wide Web!"},
			welcome:{title:'Welcome', desc:'Hello, React!'},
			contents:[
				{id:1,title:"HTML", desc:"HTML is HyperText Markup Language"},
				{id:2,title:"CSS", desc:"CSS is for design"},
				{id:3,title:"JavaScript", desc:"JavaScript is for interactive"},
			],
		};
	}
	render() {
		let _title,_desc,_article = null;
		if(this.state.mode==='welcome'){
			_title=this.state.welcome.title;
			_desc=this.state.welcome.desc;
			_article= <ReadContent title={_title} desc={_desc}></ReadContent>;
		}else if(this.state.mode==='read'){
			 for(let content of this.state.contents){
				 if(content.title!==this.state.title)
					 continue;
				 _title=content.title;
				 _desc=content.desc;
				 _article= <ReadContent title={_title} desc={_desc}></ReadContent>;
			 }
		}else if(this.state.mode==='create'){
			_article=<CreateContent></CreateContent>;
		}
		return (
			<div className="App">
				{<Subject 
					title={this.state.subject.title}
					sub={this.state.subject.sub}
					onChangePage ={function(){
						this.setState({mode:'welcome'});
					}.bind(this)}>
				</Subject>}
				<TOC data={this.state.contents}
					onChangePage={function(mode,title){
						this.setState({
							mode:mode,
							title:title
						});
					}.bind(this)}></TOC>
				<Control onChangeMode={function(mode){
						this.setState({
							mode:mode
						});
					}.bind(this)}></Control>
				{_article}
			</div>
    );
  }
}
export default App;