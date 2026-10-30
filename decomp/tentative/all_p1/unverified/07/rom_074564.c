/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x074564.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_74564(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  undefined4 unaff_D2;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  candidate_1ad6e();
  *(undefined2 *)(unaff_A6 + 0x20) = 0xffff;
  *(undefined1 *)((int)unaff_A6 + 0x8d) = 0xff;
  *unaff_A6 = 0x74580;
  bVar4 = false;
  candidate_409a();
  if (!bVar4) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_1aca2();
  bVar3 = *(byte *)(extraout_A0 + 0x97);
  if (bVar3 < 0x11) {
    return;
  }
  *(byte *)((int)unaff_A6 + 0x97) = bVar3;
  *(byte *)((int)unaff_A6 + 0x8f) = bVar3 - 0x11;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(extraout_A0 + 0x1e);
  *(undefined1 *)((int)unaff_A6 + 0x9e) = *(undefined1 *)(extraout_A0 + 0x9e);
  if (*(short *)(extraout_A0 + 0x80) != *(short *)(unaff_A6 + 0x20)) {
    *(short *)(unaff_A6 + 0x20) = *(short *)(extraout_A0 + 0x80);
    candidate_9f88();
    candidate_9ea8();
  }
  bVar3 = *(byte *)(unaff_A6[3] + 0x8d);
  uVar5 = bVar3 != 0xff;
  if (bVar3 != 0xff) {
    if ((*(byte *)((int)unaff_A6 + 0x8d) != 0xff) &&
       ((CONCAT22((short)((uint)unaff_D2 >> 0x10),
                  *(undefined2 *)((short)((ushort)*(byte *)((int)unaff_A6 + 0x8d) * 2) + 0x74730)) &
        1 << (bVar3 & 0x1f)) != 0)) {
      bVar3 = 0xff;
    }
    if (bVar3 == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
      candidate_4256();
    }
    else if (bVar3 == 1) {
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 1;
      candidate_4256();
    }
    else if (bVar3 == 2) {
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 2;
      candidate_4256();
    }
    else if (bVar3 == 3) {
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 3;
      candidate_4256();
    }
    else {
      uVar5 = bVar3 < 4;
      if (bVar3 != 4) goto LAB_00074640;
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 4;
      candidate_4256();
    }
    *(undefined1 *)(unaff_A6[3] + 0x8d) = 0xff;
    uVar5 = false;
  }
LAB_00074640:
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar5) {
    candidate_4256();
    *(undefined1 *)((int)unaff_A6 + 0x8d) = 1;
  }
  return;
}
