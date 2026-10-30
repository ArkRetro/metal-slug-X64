/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x035fd0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_35fd0(void)

{
  short sVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;

  if (*(char *)((int)unaff_A6 + 0x96) == '\x01') {
    candidate_36502();
    candidate_365f0();
  }
  bVar3 = *(byte *)((int)unaff_A6 + 0x96) < 2;
  if (*(byte *)((int)unaff_A6 + 0x96) == 2) {
    candidate_365be();
    if (bVar3) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
      candidate_4256();
      candidate_3622a();
      *unaff_A6 = 0x35f32;
      uVar4 = 0;
      candidate_1436a();
      candidate_364d2();
      if ((bool)uVar4) {
        candidate_4256();
      }
      sVar1 = *(short *)(unaff_A6 + 0x23);
      uVar4 = sVar1 == 0;
      sVar1 = sVar1 + -1;
      *(short *)(unaff_A6 + 0x23) = sVar1;
      if (sVar1 < 0) {
        candidate_3622a();
        uVar2 = candidate_29c6();
        if ((uVar2 & 1) == 0) {
          uVar4 = 0;
          candidate_4256();
        }
        else {
          uVar4 = 0;
          candidate_4256();
        }
      }
      func_0x0003623e();
      candidate_35af0();
      if (!(bool)uVar4) {
        sVar1 = *(short *)((int)unaff_A6 + 0x92) + -1;
        *(short *)((int)unaff_A6 + 0x92) = sVar1;
        if (-1 < sVar1) {
          if (_DAT_0010e99e < *(ushort *)(unaff_A6 + 0x24)) {
            return;
          }
          *(undefined2 *)(unaff_A6 + 0x24) = 0xffff;
        }
        candidate_35b1a();
        *(undefined1 *)(unaff_A6 + 0x18) = 0;
        candidate_36486();
        *unaff_A6 = candidate_35fd0;
        return;
      }
      goto LAB_00036068;
    }
    candidate_3654c();
  }
  uVar4 = *(byte *)((int)unaff_A6 + 0x96) < 3;
  if (*(byte *)((int)unaff_A6 + 0x96) == 3) {
    candidate_3658e();
    candidate_36464();
  }
  candidate_1ac38();
  candidate_364d2();
  func_0x0003623e();
  candidate_35af0();
  if (!(bool)uVar4) {
    if (*(char *)(unaff_A6 + 0x18) == '\0') {
      return;
    }
    candidate_36254();
    *(undefined2 *)((int)unaff_A6 + 0x8e) = 7;
    *unaff_A6 = candidate_3603c;
    return;
  }
LAB_00036068:
  candidate_120f8();
  if (*(char *)((int)unaff_A6 + 0x9f) == '\a') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = candidate_360a8;
  return;
}
