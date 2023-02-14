#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/errno.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Test Driver for IMDCT.");

#define DEVICE_NAME "imdct" 
#define DRIVER_NAME "imdct_driver"
#define BUFF_SIZE 576
#define BRAM_SIZE 1152
#define PI    3.141592653589793

//****************************** FUNCTION PROTOTYPES ****************************************//

static int IMDCT_open(struct inode *i, struct file *f);
static int IMDCT_close(struct inode *i, struct file *f);
static ssize_t IMDCT_read(struct file *f, char __user *buffer, size_t length, loff_t *offset);
static ssize_t IMDCT_write(struct file *f, const char __user *buffer, size_t length, loff_t *offset);
static int __init IMDCT_init(void);
static void __exit IMDCT_exit(void);

//***************************** GLOBAL VARIABLES *********************************************//

dev_t my_dev_id;
static struct class *my_class;
static struct device *my_device;
static struct cdev *my_cdev;

struct file_operations my_fops =
{
	.owner = THIS_MODULE,
	.open = IMDCT_open,
	.read = IMDCT_read,
	.write = IMDCT_write,
	.release = IMDCT_close,
};

int endRead = 0;
int endIMDCT = 0;
int q =0;
int sample = 0;
int block = 0;
int win = 0;
int brojac_xi = 0;
int zavrsio_overlap = 0;

// promenljive za IMDCT funkciju
int sine_block[4][36];
int sample_block[36];
int temp_block[36];
int block_type[2][2];
int prev_samples[2][32][18];
int x=0;
int n = 0;
int half_n = 0;
int win_count = 0;
int k = 0;
int xi = 0;
int xi_1 = 0;
int s = 0;
int cos_counter = 0;
int i = 0;

int bram_a[BRAM_SIZE];
int bram_b[BRAM_SIZE];

int samples2[2][2][576];
unsigned int start, gr, ch, ready;

int sine_block_rom_1[36] = {11796480,35127296,58195968,80740352,102760448,123994112,144179200,163315712,181403648,197918720,212860928,226492416,238026752,247988224,256114688,262144000,266076160,268173312,268173312,266076160,262144000,256114688,247988224,238026752,226492416,212860928,197918720,181403648,163315712,144179200,123994112,102760448,80740352,58195968,35127296,11796480,};
int sine_block_rom_2[36] = {11796480,35127296,58195968,80740352,102760448,123994112,144179200,163315712,181403648,197918720,212860928,226492416,238026752,247988224,256114688,262144000,266076160,268173312,268435456,268435456,268435456,268435456,268435456,268435456,266076160,247988224,212860928,163315712,102760448,35127296,0,0,0,0,0,0,};
int sine_block_rom_3[36] = {35127296,102760448,163315712,212860928,247988224,266076160,266076160,247988224,212860928,163315712,102760448,35127296,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
int sine_block_rom_4[36] = {0,0,0,0,0,0,35127296,102760448,163315712,212860928,247988224,266076160,268435456,268435456,268435456,268435456,268435456,268435456,268173312,266076160,262144000,256114688,247988224,238026752,226492416,212860928,197918720,181403648,163315712,144179200,123994112,102760448,80740352,58195968,35127296,11796480,};

int cos_rom[648] = {181352310,-212964074,-144230371,238105202,102725686,-256011458,-58100170,266138991,11708994,-268179905,35037806,262072462,-80720152,-248002149,123949803,226396048,-163413037,-197911288,163413037,-248002149,-35037806,266138991,-102725686,-212964074,212964074,102725686,-266138991,35037806,248002149,-163413037,-163413037,248002149,35037806,-266138991,102725686,212964074,144230371,-266138991,80720152,197911288,-248002149,11708994,238105202,-212964074,-58100170,262072462,-163413037,-123949803,268179905,-102725686,-181352310,256011458,-35037806,-226396048,123949803,-266138991,181352310,58100170,-248002149,226396048,-11708994,-212964074,256011458,-80720152,-163413037,268179905,-144230371,-102725686,262072462,-197911288,-35037806,238105202,102725686,-248002149,248002149,-102725686,-102725686,248002149,-248002149,102725686,102725686,-248002149,248002149,-102725686,-102725686,248002149,-248002149,102725686,102725686,-248002149,80720152,-212964074,268179905,-226396048,102725686,58100170,-197911288,266138991,-238105202,123949803,35037806,-181352310,262072462,-248002149,144230371,11708994,-163413037,256011458,58100170,-163413037,238105202,-268179905,248002149,-181352310,80720152,35037806,-144230371,226396048,-266138991,256011458,-197911288,102725686,11708994,-123949803,212964074,-262072462,35037806,-102725686,163413037,-212964074,248002149,-266138991,266138991,-248002149,212964074,-163413037,102725686,-35037806,-35037806,102725686,-163413037,212964074,-248002149,266138991,11708994,-35037806,58100170,-80720152,102725686,-123949803,144230371,-163413037,181352310,-197911288,212964074,-226396048,238105202,-248002149,256011458,-262072462,266138991,-268179905,-11708994,35037806,-58100170,80720152,-102725686,123949803,-144230371,163413037,-181352310,197911288,-212964074,226396048,-238105202,248002149,-256011458,262072462,-266138991,268179905,-35037806,102725686,-163413037,212964074,-248002149,266138991,-266138991,248002149,-212964074,163413037,-102725686,35037806,35037806,-102725686,163413037,-212964074,248002149,-266138991,-58100170,163413037,-238105202,268179905,-248002149,181352310,-80720152,-35037806,144230371,-226396048,266138991,-256011458,197911288,-102725686,-11708994,123949803,-212964074,262072462,-80720152,212964074,-268179905,226396048,-102725686,-58100170,197911288,-266138991,238105202,-123949803,-35037806,181352310,-262072462,248002149,-144230371,-11708994,163413037,-256011458,-102725686,248002149,-248002149,102725686,102725686,-248002149,248002149,-102725686,-102725686,248002149,-248002149,102725686,102725686,-248002149,248002149,-102725686,-102725686,248002149,-123949803,266138991,-181352310,-58100170,248002149,-226396048,11708994,212964074,-256011458,80720152,163413037,-268179905,144230371,102725686,-262072462,197911288,35037806,-238105202,-144230371,266138991,-80720152,-197911288,248002149,-11708994,-238105202,212964074,58100170,-262072462,163413037,123949803,-268179905,102725686,181352310,-256011458,35037806,226396048,-163413037,248002149,35037806,-266138991,102725686,212964074,-212964074,-102725686,266138991,-35037806,-248002149,163413037,163413037,-248002149,-35037806,266138991,-102725686,-212964074,-181352310,212964074,144230371,-238105202,-102725686,256011458,58100170,-266138991,-11708994,268179905,-35037806,-262072462,80720152,248002149,-123949803,-226396048,163413037,197911288,-197911288,163413037,226396048,-123949803,-248002149,80720152,262072462,-35037806,-268179905,-11708994,266138991,58100170,-256011458,-102725686,238105202,144230371,-212964074,-181352310,-212964074,102725686,266138991,35037806,-248002149,-163413037,163413037,248002149,-35037806,-266138991,-102725686,212964074,212964074,-102725686,-266138991,-35037806,248002149,163413037,-226396048,35037806,256011458,181352310,-102725686,-268179905,-123949803,163413037,262072462,58100170,-212964074,-238105202,11708994,248002149,197911288,-80720152,-266138991,-144230371,-238105202,-35037806,197911288,262072462,102725686,-144230371,-268179905,-163413037,80720152,256011458,212964074,-11708994,-226396048,-248002149,-58100170,181352310,266138991,123949803,-248002149,-102725686,102725686,248002149,248002149,102725686,-102725686,-248002149,-248002149,-102725686,102725686,248002149,248002149,102725686,-102725686,-248002149,-248002149,-102725686,-256011458,-163413037,-11708994,144230371,248002149,262072462,181352310,35037806,-123949803,-238105202,-266138991,-197911288,-58100170,102725686,226396048,268179905,212964074,80720152,-262072462,-212964074,-123949803,-11708994,102725686,197911288,256011458,266138991,226396048,144230371,35037806,-80720152,-181352310,-248002149,-268179905,-238105202,-163413037,-58100170,-266138991,-248002149,-212964074,-163413037,-102725686,-35037806,35037806,102725686,163413037,212964074,248002149,266138991,266138991,248002149,212964074,163413037,102725686,35037806,-268179905,-266138991,-262072462,-256011458,-248002149,-238105202,-226396048,-212964074,-197911288,-181352310,-163413037,-144230371,-123949803,-102725686,-80720152,-58100170,-35037806,-11708994,-268179905,-266138991,-262072462,-256011458,-248002149,-238105202,-226396048,-212964074,-197911288,-181352310,-163413037,-144230371,-123949803,-102725686,-80720152,-58100170,-35037806,-11708994,-266138991,-248002149,-212964074,-163413037,-102725686,-35037806,35037806,102725686,163413037,212964074,248002149,266138991,266138991,248002149,212964074,163413037,102725686,35037806,-262072462,-212964074,-123949803,-11708994,102725686,197911288,256011458,266138991,226396048,144230371,35037806,-80720152,-181352310,-248002149,-268179905,-238105202,-163413037,-58100170,-256011458,-163413037,-11708994,144230371,248002149,262072462,181352310,35037806,-123949803,-238105202,-266138991,-197911288,-58100170,102725686,226396048,268179905,212964074,80720152,-248002149,-102725686,102725686,248002149,248002149,102725686,-102725686,-248002149,-248002149,-102725686,102725686,248002149,248002149,102725686,-102725686,-248002149,-248002149,-102725686,-238105202,-35037806,197911288,262072462,102725686,-144230371,-268179905,-163413037,80720152,256011458,212964074,-11708994,-226396048,-248002149,-58100170,181352310,266138991,123949803,-226396048,35037806,256011458,181352310,-102725686,-268179905,-123949803,163413037,262072462,58100170,-212964074,-238105202,11708994,248002149,197911288,-80720152,-266138991,-144230371,-212964074,102725686,266138991,35037806,-248002149,-163413037,163413037,248002149,-35037806,-266138991,-102725686,212964074,212964074,-102725686,-266138991,-35037806,248002149,163413037,-197911288,163413037,226396048,-123949803,-248002149,80720152,262072462,-35037806,-268179905,-11708994,266138991,58100170,-256011458,-102725686,238105202,144230371,-212964074,-181352310,};
int cos_rom_1[216] = {163413037,-248002149,-35037806,266138991,-102725686,-212964074,102725686,-248002149,248002149,-102725686,-102725686,248002149,35037806,-102725686,163413037,-212964074,248002149,-266138991,-35037806,102725686,-163413037,212964074,-248002149,266138991,-102725686,248002149,-248002149,102725686,102725686,-248002149,-163413037,248002149,35037806,-266138991,102725686,212964074,-212964074,102725686,266138991,35037806,-248002149,-163413037,-248002149,-102725686,102725686,248002149,248002149,102725686,-266138991,-248002149,-212964074,-163413037,-102725686,-35037806,-266138991,-248002149,-212964074,-163413037,-102725686,-35037806,-248002149,-102725686,102725686,248002149,248002149,102725686,-212964074,102725686,266138991,35037806,-248002149,-163413037,163413037,-248002149,-35037806,266138991,-102725686,-212964074,102725686,-248002149,248002149,-102725686,-102725686,248002149,35037806,-102725686,163413037,-212964074,248002149,-266138991,-35037806,102725686,-163413037,212964074,-248002149,266138991,-102725686,248002149,-248002149,102725686,102725686,-248002149,-163413037,248002149,35037806,-266138991,102725686,212964074,-212964074,102725686,266138991,35037806,-248002149,-163413037,-248002149,-102725686,102725686,248002149,248002149,102725686,-266138991,-248002149,-212964074,-163413037,-102725686,-35037806,-266138991,-248002149,-212964074,-163413037,-102725686,-35037806,-248002149,-102725686,102725686,248002149,248002149,102725686,-212964074,102725686,266138991,35037806,-248002149,-163413037,163413037,-248002149,-35037806,266138991,-102725686,-212964074,102725686,-248002149,248002149,-102725686,-102725686,248002149,35037806,-102725686,163413037,-212964074,248002149,-266138991,-35037806,102725686,-163413037,212964074,-248002149,266138991,-102725686,248002149,-248002149,102725686,102725686,-248002149,-163413037,248002149,35037806,-266138991,102725686,212964074,-212964074,102725686,266138991,35037806,-248002149,-163413037,-248002149,-102725686,102725686,248002149,248002149,102725686,-266138991,-248002149,-212964074,-163413037,-102725686,-35037806,-266138991,-248002149,-212964074,-163413037,-102725686,-35037806,-248002149,-102725686,102725686,248002149,248002149,102725686,-212964074,102725686,266138991,35037806,-248002149,-163413037,};

//*******************************FUNCTIONS*************************************//
static int IMDCT_open(struct inode *i, struct file *f)
{
  printk(KERN_INFO "IMDCT opened\n");
  return 0;
}

static int IMDCT_close(struct inode *i, struct file *f)
{
  printk(KERN_INFO "IMDCT closed\n");
  return 0;
}

void getIMDCT(void){

    for ( i = 0; i < n; i++) {
        xi = 0;
        for ( k = 0; k < half_n; k++) {
            
            s = samples2[gr][ch][18 * block + half_n * win + k];

            if( half_n == 18 ){
                xi_1 = s * cos_rom[k + ( i * 18 )];
                xi = xi + xi_1;

            }else if(half_n == 6){
                xi_1 = s * cos_rom[k + ( i * 6 )];
                xi = xi + xi_1;
            }else{
                printk(KERN_ALERT"GRESKA: half_n nije ni 6 ni 18");
            }

        }

        // Windowing samples 
        sample_block[win * n + i] = xi * sine_block[block_type[gr][ch]][i];

    }

}

ssize_t IMDCT_read(struct file *f, char __user *buffer, size_t length, loff_t *offset) 
{
    char buff[BRAM_SIZE]; 
    int minor;
    int value;
    int ret, len;
    ssize_t buff_size;
    ssize_t bram_a_size;
    minor = MINOR(f->f_inode->i_rdev);
    printk("IMDCT read: %d minor\n", minor);
    
    if(endRead == 1){
        endRead = 0;
        return 0;
    }

    if(minor == 0){ // citamo iz bram_a
        
        value = bram_a[q];
        len = scnprintf(buff,BRAM_SIZE, "%d,", value);
        *offset += len;

        printk(KERN_NOTICE"bram_a[%d] = %s", q, buff);
        ret = copy_to_user(buffer, buff, len);
        if(ret){
            return -EFAULT;
		}

        q++;
        if( q > BRAM_SIZE ){
            endRead =1;
            q = 0;
        }

    }   

    if(minor == 1){ //citamo iz bram_b

        value = bram_b[q];
        len = scnprintf(buff,BRAM_SIZE, "%d,", value);
        *offset += len;
        printk(KERN_NOTICE"bram_b[%d] = %s", q, buff);
        ret = copy_to_user(buffer, buff, len);
        if(ret){
            return -EFAULT;
		}

        q++;
        if( q > BRAM_SIZE ){
            endRead =1;
            q = 0;
        }
       
    }

    if(minor == 2){ //citamo iz IMDCT
        printk(KERN_WARNING"start=%u\n", start);
        printk(KERN_WARNING"block_type_00=%u\n", block_type[0][0]);
        printk(KERN_WARNING"block_type_01=%u\n", block_type[0][1]);
        printk(KERN_WARNING"block_type_10=%u\n", block_type[1][0]);
        printk(KERN_WARNING"block_type_11=%u\n", block_type[1][1]);
        printk(KERN_WARNING"gr=%u\n", gr);
        printk(KERN_WARNING"ch=%u\n", ch);
        printk(KERN_WARNING"ready=%u\n", ready);
    }

    return len;

}

ssize_t IMDCT_write(struct file *f, const char __user *buffer, size_t length, loff_t *offset) 
{
	char buff[BRAM_SIZE];
    char buff_1[10];
    int minor;
    int ret;
	int vrednost = 0;
	int case_vr = 0;
	char string[40];
    char start_1[] = "start", block_type_00_1[]="block_type_00", block_type_01_1[]="block_type_01", block_type_10_1[]="block_type_10", block_type_11_1[]="block_type_11", gr_1[]="gr", ch_1[]="ch", ready_1[]="ready";
    unsigned int bram_pos=0;
	unsigned int value=0;
    char __user *p = buffer;

    //promenljive za bram 0
    int a = 0;
    int j = 0;
    int m = 0;
    int len;

    printk("IMDCT write\n");
    minor = MINOR(f->f_inode->i_rdev);
    ret = copy_from_user(buff, p, length); 
    
    if(ret){
        printk("copy from user failed \n");
        return -EFAULT;
    } 
    buff[length] = '\0';

    printk(KERN_NOTICE"buff = %s",buff);

    if(minor == 0){ //ako upisujemo u bram_a
    
        sscanf(buff, "[%d] = %d", &bram_pos, &value);

        if (bram_pos > BRAM_SIZE)
        {
            printk(KERN_WARNING"bram_a: position exceeded maximum value \n");
        }
        else
        {
            bram_a[bram_pos] = value;
        }

        for(j = 0; j < 576; j++ ){
                samples2[0][0][j] = bram_a[j];
        }
        for(j = 576; j < 1152; j++ ){
                samples2[0][1][j] = bram_a[j];
        }
  
        printk("MINOR 0 write\n");
        return length;
    }

    if(minor == 1){ //ako upisujemo u bram_b
    
        sscanf(buff, "[%d] = %d", &bram_pos, &value);

        if (bram_pos > BRAM_SIZE)
        {
            printk(KERN_WARNING"bram_b: position exceeded maximum value \n");
        }
        else
        {
            bram_a[bram_pos] = value;
        }

        for(j = 0; j < 576; j++ ){
                samples2[0][0][j] = bram_b[j];
        }
        for(j = 576; j < 1152; j++ ){
                samples2[0][1][j] = bram_b[j];
        }
        printk("MINOR 1 write\n");
        return length;
    }
    
    if(minor == 2){
        sscanf(buff, "%s = %d", string, &vrednost);
			
		if(strcmp(string, start_1)==0){
			case_vr = 1;
		}else if(strcmp(string, block_type_00_1)==0){
			case_vr = 2;
		}else if(strcmp(string, block_type_01_1)==0){
			case_vr = 3;
		}else if(strcmp(string, block_type_10_1)==0){
			case_vr = 4;
		}else if(strcmp(string, block_type_11_1)==0){
			case_vr = 5;
		}else if(strcmp(string, gr_1)==0){
			case_vr = 6;
		}else if(strcmp(string, ch_1)==0){
			case_vr = 7;
		}else{
			printk(KERN_ALERT"Ne postoji registar");
		}

        switch(case_vr){

			case 1:			
				if(vrednost!=0 && vrednost!=1){
					printk(KERN_WARNING"IP:start mora biti 1 ili 0\n");
				}else{
					start = vrednost;
                    printk(KERN_WARNING"Vrednost start signala je: %d\n", vrednost);
			}break;
			
            case 2:
				if(vrednost!=0 && vrednost!=1 && vrednost!=2 && vrednost!=3){
					printk(KERN_WARNING"IP:block_type_00 mora biti 0 ili 1 ili 2 ili 3\n");
				}else{
                    block_type[0][0] = vrednost;
					printk(KERN_WARNING"Vrednost block_type_00 signala je: %d\n", vrednost);
			}break;

			case 3:
				if(vrednost!=0 && vrednost!=1 && vrednost!=2 && vrednost!=3){
					printk(KERN_WARNING"IP:block_type_01 mora biti 0 ili 1 ili 2 ili 3\n");
				}else{
					block_type[0][1] = vrednost;
					printk(KERN_WARNING"Vrednost block_type_01 signala je: %d\n", vrednost);
			}break;

			case 4:
				if(vrednost!=0 && vrednost!=1 && vrednost!=2 && vrednost!=3){
					printk(KERN_WARNING"IP:block_type_10 mora biti 0 ili 1 ili 2 ili 3\n");
				}else{
					block_type[1][0] = vrednost;
					printk(KERN_WARNING"Vrednost block_type_10 signala je: %d\n", vrednost);
			}break;

			case 5:
				if(vrednost!=0 && vrednost!=1 && vrednost!=2 && vrednost!=3){
					printk(KERN_WARNING"IP:block_type_11 mora biti 0 ili 1 ili 2 ili 3\n");
				}else{
					block_type[1][1] = vrednost;
					printk(KERN_WARNING"Vrednost block_type_11 signala je: %d\n", vrednost);
			}break;

			case 6:
				if(vrednost!=0 && vrednost!=1){
					printk(KERN_WARNING"IP:GR mora biti 1 ili 0\n");
				}else{
					gr = vrednost;
					printk(KERN_WARNING"Vrednost GR signala je: %d\n", vrednost);
			}break;
			
			case 7:
				if(vrednost!=0 && vrednost!=1){
					printk(KERN_WARNING"IP:CH mora biti 1 ili 0\n");
				}else{
					ch = vrednost;
					printk(KERN_WARNING"Vrednost CH signala je: %d\n", vrednost);
			}break;
        }

        if(start ==1){
            printk(KERN_ALERT"POCINJE OBRADA");

            ready = 1;
            printk(KERN_WARNING"Start: %d\nReady: %d\n", start, ready);

            for( j = 0; j < 576; j++ ){
                sine_block[0][j] = sine_block_rom_1[j];
            }
            for( j = 0; j < 576; j++ ){
                sine_block[1][j] = sine_block_rom_2[j];
            }
            for( j = 0; j < 576; j++ ){
                sine_block[2][j] = sine_block_rom_3[j];
            }
            for( j = 0; j < 576; j++ ){
                sine_block[3][j] = sine_block_rom_4[j];
            }
            
            if (block_type[gr][ch] == 2){
                n = 12;
                win_count = 3;
            } else{
                n = 36;
                win_count = 1;
            }
	        half_n = n / 2;

            for( sample = 0; sample < 576 ;sample += 18){
                for ( block = 0; block < 32; block++) {
                    for ( win = 0; win < win_count; win++) {    
                        getIMDCT();
                    }
                } 

                if (block_type[gr][ch] == 2) {
	    	    	
	    	    	for ( x=0;x<36;x++)
	    	    		temp_block[x]=sample_block[x];

	    	    	for (i = 0; i < 6; i++)
	    	    		sample_block[i] = 0;
	    	    	for (i = 6; i < 12; i++)
	    	    		sample_block[i] = temp_block[0 + i - 6];
	    	    	for (i = 12; i < 18; i++)
	    	    		sample_block[i] = temp_block[0 + i - 6] + temp_block[12 + i - 12];
	    	    	for (i = 18; i < 24; i++)
	    	    		sample_block[i] = temp_block[12 + i - 12] + temp_block[24 + i - 18];
	    	    	for (i = 24; i < 30; i++)
	    	    		sample_block[i] = temp_block[24 + i - 18];
	    	    	for (i = 30; i < 36; i++)
	    	    		sample_block[i] = 0;
	    	    }
    
                // Overlap
	    	    for ( i = 0; i < 18; i++) {
	    	    	samples2[gr][ch][sample + i] = sample_block[i] + prev_samples[ch][block][i];
	    	    	prev_samples[ch][block][i] = sample_block[18 + i];
	    	    }
            }

                for(i = 0; i < 576; i++){
                    bram_a[i] = samples2[0][0][i];
                    bram_b[i] = samples2[1][0][i];
                }   
                for(i = 576; i < 1152; i++){
                    bram_a[i] = samples2[0][1][i]; 
                    bram_b[i] = samples2[1][1][i];
                }
            start = 0;
            printk(KERN_ALERT"ZAVRSENA OBRADA");
                
        }else{
            ready = 0;
        }
        return length;    
    }
}

static int __init IMDCT_init(void)
{
    int ret = 0;
	
    ret = alloc_chrdev_region(&my_dev_id, 0, 3, "IMDCT");
    if (ret){
        printk(KERN_ERR "failed to register char device\n");	
        return ret;
    }
    printk(KERN_INFO "char device region allocated\n");

    my_class = class_create(THIS_MODULE, "IMDCT_class");
    if (my_class == NULL){
        printk(KERN_ERR "failed to create class\n");
        goto fail_0;
    }
    printk(KERN_INFO "class created\n");

    if (device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id), 0), NULL, "bram_a") == NULL)
    {
        printk(KERN_ERR "failed to create device\n");
        goto fail_1;
    }
    printk(KERN_INFO "device created bram_a\n");

    if (device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id), 1), NULL, "bram_b") == NULL)
    {
        printk(KERN_ERR "failed to create device\n");
        goto fail_1;
    }
    printk(KERN_INFO "device created bram_b\n");

    if (device_create(my_class, NULL, MKDEV(MAJOR(my_dev_id), 2), NULL, "IMDCT") == NULL)
    {
        printk(KERN_ERR "failed to create device\n");
        goto fail_1;
    }
    printk(KERN_INFO "device created IMDCT\n");

    my_cdev = cdev_alloc();
	my_cdev->ops = &my_fops;
	my_cdev->owner = THIS_MODULE;

    if (cdev_add(my_cdev, my_dev_id, 3) == -1)  
	{
        printk(KERN_ERR "failed to add cdev\n");
		goto fail_2;
	}
    printk(KERN_INFO "cdev added\n");

    return 0;
 
   fail_2:
        device_destroy(my_class, my_dev_id );
   fail_1:
        class_destroy(my_class);
   fail_0:
        unregister_chrdev_region(my_dev_id, 1);
    return -1;
}

static void __exit IMDCT_exit(void)
{
  cdev_del(my_cdev);
  device_destroy(my_class, MKDEV(MAJOR(my_dev_id),0));
  device_destroy(my_class, MKDEV(MAJOR(my_dev_id),1));
  device_destroy(my_class, MKDEV(MAJOR(my_dev_id),2));
  class_destroy(my_class);
  unregister_chrdev_region(my_dev_id, 3);
  printk(KERN_INFO "IMDCT driver closed.\n");
}

module_init(IMDCT_init);
module_exit(IMDCT_exit);