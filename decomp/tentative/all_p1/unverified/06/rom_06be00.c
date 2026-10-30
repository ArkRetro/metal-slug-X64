/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06be00.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6be00(void)

{
  undefined2 uVar1;
  short sVar2;
  ushort uVar3;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) & 0xfe;
  candidate_6cc68();
  if ((*(byte *)((int)unaff_A6 + 0x8d) & 2) == 0) {
    uVar1 = candidate_260c8();
  }
  else {
    uVar1 = candidate_260c8();
  }
  *(undefined2 *)((int)unaff_A6 + 0x86) = uVar1;
  *unaff_A6 = 0x6be3a;
  sVar2 = *(short *)((int)unaff_A6 + 0x86);
  uVar4 = sVar2 == 0;
  sVar2 = sVar2 + -1;
  *(short *)((int)unaff_A6 + 0x86) = sVar2;
  if (sVar2 < 0) {
    sVar2 = candidate_6cb24();
    *(char *)(unaff_A6 + 0x23) = (char)sVar2;
                    /* WARNING: Switch is manually overridden */
    switch(sVar2 * 4) {
    case 0:
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x92) = 0;
      *unaff_A6 = candidate_6be92;
      uVar4 = 0;
      candidate_6ca80();
      candidate_427a();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_6beaa;
        uVar4 = 0;
      }
      break;
    case 4:
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x92) = 0;
      *unaff_A6 = candidate_6bf5a;
      uVar4 = 0;
      candidate_6ca80();
      candidate_427a();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_6bf72;
        uVar4 = 0;
      }
      break;
    case 8:
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
      *(undefined1 *)((int)unaff_A6 + 0x92) = 0;
      *unaff_A6 = candidate_6c022;
      uVar4 = 0;
      candidate_6ca80();
      candidate_427a();
      if ((bool)uVar4) {
        *unaff_A6 = candidate_6c03a;
        uVar4 = 0;
      }
    }
  }
  else {
    candidate_6ca5c();
    candidate_6ca80();
    candidate_427a();
  }
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0xff;
    uVar3 = candidate_260c8();
    uVar4 = uVar3 < *(ushort *)((int)unaff_A6 + 0x4e);
    if ((short)*(ushort *)((int)unaff_A6 + 0x4e) < (short)uVar3) {
      *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) & 0xfd;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_6c0d8;
    uVar4 = 0;
  }
  candidate_1945c();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_6c14c;
  }
  candidate_140c0();
  return;
}
