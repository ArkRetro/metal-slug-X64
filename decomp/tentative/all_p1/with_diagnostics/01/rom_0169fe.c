/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0169fe.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */

void candidate_169fe(void)

{
  char cVar1;
  char cVar2;
  uint uVar3;
  code cVar4;
  uint extraout_D1;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar5;
  bool bVar6;

  uVar3 = candidate_66e5c();
  cVar4 = (code)0x0;
  if ((uVar3 & 1) != 0) {
    cVar4 = candidate_4364[unaff_A5 + 7];
  }
  if ((uVar3 & 2) != 0) {
    cVar4 = (code)(*(byte *)(unaff_A5 + 0x4371) | (byte)cVar4);
  }
  if ((((byte)cVar4 & 0xf0) == 0) && (*(char *)(unaff_A6 + 0x18) == '\0')) {
LAB_00016a3e:
    if ((*(char *)(unaff_A5 + 0x7db6) != '\x01') && (*(char *)(unaff_A5 + 0x7db6) != '\x01'))
    goto LAB_00016a52;
  }
  else {
    DAT_0010f003 = 3;
    candidate_15418();
    func_0x00016502();
    candidate_12244();
    candidate_16d80();
    candidate_3d4ce();
    *(undefined1 *)(unaff_A6 + 0x18) = 0;
    candidate_40bc();
    *unaff_A6 = 0x16954;
    cVar1 = *(char *)(unaff_A6 + 0x18);
    if (cVar1 == '\0') goto LAB_00016a3e;
    candidate_9c0a();
    if (cVar1 == '\x02') {
      DAT_0010f003 = 0;
      DAT_0010c2e7 = '\0';
      candidate_c8ce6();
      candidate_16d80();
      func_0x00016502();
      candidate_1aa28();
      candidate_3d4ac();
      candidate_2976();
      candidate_12244();
      candidate_40bc();
      *(undefined1 *)(unaff_A6 + 0x18) = 0;
      *unaff_A6 = candidate_169fe;
      goto LAB_00016a3e;
    }
    if (cVar1 != '\x01') goto LAB_00016a3e;
    DAT_0010f003 = 3;
    *unaff_A6 = 0x16990;
    if (DAT_00100088 != '\x02') goto LAB_00016a52;
  }
  candidate_17f20();
  candidate_2852();
  DAT_0010c2e7 = '\0';
  DAT_0010f003 = 3;
  candidate_3cad2();
  candidate_2976();
  if ((DAT_0010fd82 != '\0') && (DAT_0010fd8e != '\0')) {
    candidate_c8ce6();
    candidate_16d80();
    func_0x00016502();
    candidate_1aa28();
    candidate_3d4ac();
    candidate_12244();
    candidate_40bc();
    *(undefined1 *)(unaff_A6 + 0x18) = 0;
    *unaff_A6 = 0x16c20;
    candidate_190bc();
    if (((extraout_D1 & 0xf0) == 0) && (*(char *)(unaff_A6 + 0x18) == '\0')) goto LAB_00016a52;
  }
  func_0x00016502();
  candidate_16d80();
  candidate_3d4ac();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  candidate_cab9c();
  *unaff_A6 = 0x16c60;
  if (*(char *)(unaff_A6 + 0x18) != '\0') {
    candidate_3d4ac();
    *(undefined1 *)(unaff_A6 + 0x18) = 0;
    candidate_a8686();
    *unaff_A6 = 0x16c80;
    if (*(char *)(unaff_A6 + 0x18) != '\0') {
      candidate_15422();
      candidate_16d80();
      candidate_1aa28();
      candidate_3d4ac();
      func_0x00016502();
      candidate_17fd0();
      candidate_12244();
      func_0x0003cf12();
      candidate_c8eaa();
      candidate_c8daa();
      candidate_c8cd6();
      candidate_40bc();
      *(undefined1 *)(unaff_A6 + 0x18) = 0;
      *unaff_A6 = 0x16cf4;
      if (*(char *)(unaff_A6 + 0x18) != '\0') {
        candidate_4192();
        func_0x00016502();
        candidate_c8df0();
        candidate_17f4c();
        *unaff_A6 = 0x16d18;
        bVar6 = false;
        func_0x000040ac();
        if (!bVar6) {
          candidate_16d80();
          *(undefined1 *)(unaff_A6 + 0x18) = 0;
          candidate_cac24();
          *unaff_A6 = 0x16d3c;
          if (*(char *)(unaff_A6 + 0x18) != '\0') {
            candidate_16d80();
            func_0x00016502();
            candidate_12244();
            *(undefined1 *)(unaff_A6 + 0x18) = 0;
            bVar6 = false;
            candidate_40bc();
            if ((!bVar6) || (*unaff_A6 = 0x16d72, *(char *)(unaff_A6 + 0x18) != '\0')) {
                    /* WARNING: Bad instruction - Truncating control flow here */
              halt_baddata();
            }
          }
        }
      }
    }
  }
LAB_00016a52:
  cVar1 = DAT_0010c2e6;
  if (DAT_0010c2e7 == '\0') {
    cVar2 = *(char *)((int)unaff_A6 + 0x72);
    *(char *)((int)unaff_A6 + 0x72) = DAT_0010c2e6;
    if (cVar1 != '\0') {
      uVar5 = 0;
      if (cVar2 == '\0') {
        func_0x00016572();
      }
      candidate_354e();
      if ((bool)uVar5) {
        candidate_1a7f4();
      }
    }
  }
  return;
}
