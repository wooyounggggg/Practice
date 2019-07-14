import React, { Component } from 'react';

class TOC extends Component{
	render(){
		let lis = [];
		let data = this.props.data;
		for(let i=0;i<data.length;i++){
			lis.push(<li key={data[i].id}>
					<a 
						href={"/content/"+data[i].id} 
						onClick={function(e){
							e.preventDefault();
							this.props.onChangePage("read",data[i].title);
						}.bind(this)}
					>{data[i].title}</a>
					</li>);
		} 
      return (
        <nav>
            <ul>
                {lis}
            </ul>
        </nav>
      );
    }
  }

export default TOC;