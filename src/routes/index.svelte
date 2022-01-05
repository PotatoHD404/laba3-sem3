<script>
  import Input from '../components/input.svelte';
  import Select from '../components/select.svelte';
  import Field from '../components/field.svelte';
  import Button from '../components/button.svelte';
  // import LabWorker from '../../scripts/actual-service-worker.js?url';
  import LabWorker1 from '../../scripts/actual-service-worker.js?worker';
  import { onMount } from 'svelte';

  let worker;
  let ok = false;
  let consoleText = '';
  let type_selected = false;
  let sequence = '';
  let result = '';
  let inputValue = ['', '', 'True', 'True'];
  onMount(async () => {
    // worker = new Worker(LabWorker);
    worker = new LabWorker1();
    worker.onmessage = (e) => {
      if (e && e.data) {
        print(e.data);
      }
    };
    worker.postMessage('init');
    ok = true;
    // console.log('initialized');
  });


  // fetchWorker.onmessage = ({ data: { status, data } }) => {
  //   if (status) loadState.status = status;
  //   if (data) dataParsed = data;
  //   if (status && status === "done") fetchWorker.terminate();
  // };

  // worker.postMessage('init');

  function Command(input, choice) {

    if (ok) {
      ok = false;
      switch (input) {
        case 'init':
          // console.log(inputValue);
          let message = '1' + '\n';
          if (inputValue[0] === '')
            message += '0';
          else
            message += inputValue[0];
          message += '\n';
          if (inputValue[1] === '')
            message += '0';
          else
            message += inputValue[1];
          message += '\n';
          if (inputValue[2] === 'True')
            message += '1';
          else
            message += '2';
          message += '\n';
          if (inputValue[3] === 'True')
            message += '1';
          else
            message += '2';
          message += '\n' + '9' + '\n';

          worker.postMessage(message);
          break;
        case 'colorize':
          // console.log('2' + '\n' + choice + '\n' + '4');
          worker.postMessage('8' + '\n' + '9' + '\n');
          break;
        case 'top':
          worker.postMessage('6' + '\n' + '9' + '\n');
          break;
        case 'add':
          worker.postMessage('2' + '\n' + '9' + '\n');
          break;
        case 'remove':
          if (choice !== '')
            worker.postMessage('2' + '\n' + choice + '\n');
          break;
      }
    }
  }

  function print(data) {
    ok = true;
    // console.log(data);
    if (data.includes('graph {'))
      sequence = data.split('Sequence: ')[1];
    else if (data.includes('Result: '))
      result = data.split('Result: ')[1];

    consoleText += data + '\r\n';
    let textarea = document.getElementById('consoleOutput');
    let temp = textarea.scrollTop;
    let interval = setInterval(() => {
      let end = textarea.scrollHeight;
      if (temp < end) {
        textarea.scrollTop += 50;
        temp += 50;
      } else {
        clearInterval(interval);
      }
    }, 5);

  }

</script>

<div class='flex justify-center'>
  <div class='flex justify-center md:w-full xl:w-2/3 2xl:w-1/2'>
    <div class='flex justify-center flex-wrap md:w-2/3 w-full'>
    <textarea id='consoleOutput'
              class='px-2 py-2 flex console bg-light ring-1 ring-outline-light dark:bg-deep-black dark:text-gray-300
       rounded-md w-full focus:outline-none h-44 dark:ring-outline-dark m-1'
              readonly>{consoleText}</textarea>


      <div class='my-2 w-full flex flex-wrap justify-center' id='menu'>
        <Input text='Nodes count' command={(choice)=>{Command('input',choice);}}
               bind:choice={inputValue[0]} />
        <Input text='Edges count' command={(choice)=>{Command('remove',choice);}}
               bind:choice={inputValue[1]} />
        <Select text='Directed' command={(choice)=>{Command('init',choice);}}
                options={['True', 'False']} bind:choice={inputValue[2]} />
        <Select text='Edge weighted' command={()=>{Command('init');}}
                button_text='Init' options={['True', 'False']} bind:choice={inputValue[3]} />
        <div class='flex justify-center flex-wrap md:w-2/3 w-full'>
          <Button button_text='Colorize' command={(choice)=>{Command('sort',choice);}} />
          <Button button_text='Topological sort' command={(choice)=>{Command('sort',choice);}} />
          <Button button_text='Add node' command={(choice)=>{Command('sort',choice);}} />
        </div>
        <Input text='Node index' command={(choice)=>{Command('input',choice);}}
               button_text='Remove' />
        <Field label_text='Result' text={result} />
        <Field label_text='Graph' text={sequence} />
      </div>
    </div>


  </div>
</div>


<style>
    /*.greenBox {*/
    /*    background-color: rgba(0, 255, 0, 0.2);*/
    /*}*/

    label {
        top: 0;
        transform: translateY(-100%);
        font-size: 11px;
    }

    :global(body) {
        @apply m-auto transition-colors duration-300 bg-dark;
    }

    :global(body.light) {
        @apply bg-white;
    }

</style>

