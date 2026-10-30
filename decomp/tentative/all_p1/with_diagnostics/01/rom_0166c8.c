/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0166c8.
 * Status: tentative_c_with_diagnostics. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_166c8(void)

{
  char cVar1;
  char cVar2;
  int iVar3;
  short sVar4;
  ushort uVar5;
  uint extraout_D1;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;
  undefined1 uVar7;

  sVar4 = candidate_9baa();
  if (sVar4 != 0) {
    return;
  }
  *unaff_A6 = 0x166dc;
  candidate_3d4ac();
  candidate_2976();
  DAT_0010f003 = 3;
  candidate_17fd0();
  if (DAT_0010fdae == '\0') {
    halt_baddata();
  }
  if (DAT_0010fdae == '\x01') {
    halt_baddata();
  }
  if (DAT_0010fdae == '\x03') {
    candidate_16502();
    candidate_40bc();
    *(undefined1 *)(unaff_A6 + 0x18) = 0;
    *unaff_A6 = 0x1691c;
LAB_000168da:
    candidate_16648();
  }
  else if (_DAT_00000180 == 0) {
    candidate_16502();
    candidate_16d80();
    candidate_3d4ac();
    candidate_3e2b8();
    *unaff_A6 = 0x1674a;
    if (*(char *)(unaff_A6 + 0x18) != '\0') {
      candidate_16502();
      candidate_16d80();
      candidate_3d4ac();
      bVar6 = false;
      candidate_40bc();
      *(char *)(unaff_A6 + 0x18) = -!bVar6;
      *unaff_A6 = 0x1677a;
      if (*(char *)(unaff_A6 + 0x18) == '\0') goto LAB_000168da;
      *unaff_A6 = 0x167a0;
      goto LAB_000167a0;
    }
  }
  else {
LAB_000167a0:
    DAT_0010f003 = 3;
    DAT_0010c2e7 = '\0';
    candidate_16d80();
    candidate_1aa28();
    candidate_16502();
    candidate_12244();
    candidate_2976();
    candidate_3b58();
    uVar5 = *(ushort *)(unaff_A5 + 0x700e);
    if (1 < uVar5) {
      uVar5 = 0;
      *(undefined2 *)(unaff_A5 + 0x700e) = 0;
    }
    iVar3 = (int)(short)(uVar5 * 10);
    func_0x00017f2c();
    if (-1 < *(short *)(iVar3 + 0x16792)) {
      candidate_41b0();
      *(undefined2 *)(extraout_A0 + 0x70) = 0;
      *(undefined2 *)(extraout_A0 + 0x6e) = *(undefined2 *)(iVar3 + 0x16792);
    }
    if (-1 < *(short *)(iVar3 + 0x16794)) {
      candidate_41b0();
      *(undefined2 *)(extraout_A0_00 + 0x70) = 1;
      *(undefined2 *)(extraout_A0_00 + 0x6e) = *(undefined2 *)(iVar3 + 0x16794);
    }
    candidate_17f4c();
    candidate_3cad2();
    *(short *)(unaff_A5 + 0x700e) = *(short *)(unaff_A5 + 0x700e) + 1;
    *(undefined2 *)((int)unaff_A6 + 0x6e) = 900;
    *unaff_A6 = 0x1687c;
    sVar4 = *(short *)((int)unaff_A6 + 0x6e);
    bVar6 = sVar4 == 0;
    *(short *)((int)unaff_A6 + 0x6e) = sVar4 + -1;
    if ((!bVar6) && (candidate_3cabc(), bVar6)) goto LAB_000168da;
    candidate_16d80();
    candidate_1aa28();
    candidate_16502();
    candidate_12244();
    *(undefined1 *)(unaff_A6 + 0x18) = 0;
    bVar6 = false;
    candidate_40bc();
    if (!bVar6) {
      halt_baddata();
    }
    *unaff_A6 = 0x168cc;
    if (*(char *)(unaff_A6 + 0x18) != '\0') {
      halt_baddata();
    }
  }
  if (DAT_00100088 != '\x02') goto LAB_00016a52;
  if (DAT_0010fd82 == '\0') {
    candidate_15418();
    candidate_16502();
    candidate_12244();
    candidate_16d80();
    candidate_3d4ce();
    *(undefined1 *)(unaff_A6 + 0x18) = 0;
    candidate_40bc();
    *unaff_A6 = 0x16954;
    cVar1 = *(char *)(unaff_A6 + 0x18);
    if (cVar1 != '\0') {
      candidate_9c0a();
      if (cVar1 == '\x02') {
        DAT_0010f003 = 0;
        DAT_0010c2e7 = '\0';
        candidate_c8ce6();
        candidate_16d80();
        candidate_16502();
        candidate_1aa28();
        candidate_3d4ac();
        candidate_2976();
        candidate_12244();
        candidate_40bc();
        *(undefined1 *)(unaff_A6 + 0x18) = 0;
        *unaff_A6 = candidate_169fe;
      }
      else if (cVar1 == '\x01') {
        DAT_0010f003 = 3;
        *unaff_A6 = 0x16990;
        if (DAT_00100088 != '\x02') goto LAB_00016a52;
        goto LAB_00016b9a;
      }
    }
    if ((*(char *)(unaff_A5 + 0x7db6) != '\x01') && (*(char *)(unaff_A5 + 0x7db6) != '\x01'))
    goto LAB_00016a52;
  }
LAB_00016b9a:
  func_0x00017f20();
  candidate_2852();
  DAT_0010c2e7 = '\0';
  DAT_0010f003 = 3;
  candidate_3cad2();
  candidate_2976();
  if ((DAT_0010fd82 != '\0') && (DAT_0010fd8e != '\0')) {
    candidate_c8ce6();
    candidate_16d80();
    candidate_16502();
    candidate_1aa28();
    candidate_3d4ac();
    candidate_12244();
    candidate_40bc();
    *(undefined1 *)(unaff_A6 + 0x18) = 0;
    *unaff_A6 = 0x16c20;
    candidate_190bc();
    if (((extraout_D1 & 0xf0) == 0) && (*(char *)(unaff_A6 + 0x18) == '\0')) goto LAB_00016a52;
  }
  candidate_16502();
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
      candidate_16502();
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
        candidate_16502();
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
            candidate_16502();
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
      uVar7 = 0;
      if (cVar2 == '\0') {
        func_0x00016572();
      }
      candidate_354e();
      if ((bool)uVar7) {
        candidate_1a7f4();
      }
    }
  }
  return;
}
