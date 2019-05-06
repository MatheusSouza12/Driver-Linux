#include <linux/module.h>
#include <linux/kernel.h>	
#include <linux/usb.h>

static struct usb_device *device;

//Uma interface corresponde à funcionalidade fornecida pelo dispositivo.
//Decodificar as informações do dispositivo
static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    //Estrutura de dados -> usb_host_interface & usb_endpoint_descriptor
    //Representa a descrição do padrão usb
    struct usb_host_interface *iface_desc; //ponteiro
    struct usb_endpoint_descriptor *endpoint; //ponteiro
    int i;

    //Altsetting -> Matriz de estruturas de interface, contendo todas as configurações alternativas que podem ser selecionadas para essa interface. Consiste em um conjuto de configurações do terminal
    iface_desc = interface->cur_altsetting;
    
    printk(KERN_INFO "[*] HD Externo MatheusSouza (%04X:%04X) plugado\n", id->idVendor, id->idProduct);
    printk("HD Externo Matheus Souza");
    printk(KERN_INFO "ID->bNumEndpoints: %02X\n",
            iface_desc->desc.bNumEndpoints);
    printk(KERN_INFO "ID->bInterfaceClass: %02X\n",
            iface_desc->desc.bInterfaceClass);

     //bNumEndpoint -> Número de pontos de extremidade usados por essa interface
    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n",
                i, endpoint->bEndpointAddress);
        printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n",
                i, endpoint->bmAttributes);
        //Tamanho máximo do Pacote que este endpoint é capaz de enviar ou receber        
        printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n",
                i, endpoint->wMaxPacketSize,
                endpoint->wMaxPacketSize);
        printk(KERN_INFO "ED[%d]->bLength: 0x%02X\n",
        i, endpoint->bLength);
        printk(KERN_INFO "ED[%d]->bDescriptorType: 0x%02X\n",
        i, endpoint->bDescriptorType);
 
    }
    //Obter o identificador do dispositivo
    device = interface_to_usbdev(interface);
    return 0;
}
// disconnect
static void pen_disconnect(struct usb_interface *interface)
{
        printk(KERN_INFO "[*] HD Externo MatheusSouza removido\n");
}

static struct usb_device_id pen_table[] = {
    //idFornecedor e Id Produto
    { USB_DEVICE(0x152d,0x2329) },
    {},
};

//O driver expõe suas informações usando a API MODULE_DEVICE_TABLE
//No momento da compilação, o processo de compilação extrai essas informações do driver e cria uma tabela
//Quando o dispositivo esta conectado, o kernel verifica o device table pra ver se algum driver está usando o device id em questão, se sim, inicializa o dispositivo
MODULE_DEVICE_TABLE(usb_device_id, pen_table);

//definição basica do driver
static struct usb_driver pen_driver =
{
    .name = "HD Matheussouza",
    .id_table = pen_table, // tabela de identificação
    .probe = pen_probe, //sonda
    .disconnect = pen_disconnect, //limpar a memoria
};


static int __init pen_init(void)
{
        int ret = -1;
        printk(KERN_INFO "[*] MatheusSouza - Construtor do driver [*] \n");
        printk(KERN_INFO "\tRegistrando driver no kernel\n");
        ret = usb_register(&pen_driver);
        printk(KERN_INFO "\tRegistro concluído\n");
        return ret;
}

static void __exit pen_exit(void)
{
        printk(KERN_INFO "[*] MatheusSouza - Desconstrutor do driver[*]\n");
        usb_deregister(&pen_driver);
        printk(KERN_INFO "\tdesregistro concluído\n");
}

module_init(pen_init);
module_exit(pen_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("MatheusSouza");
MODULE_DESCRIPTION("InfraEstrutura de Software");