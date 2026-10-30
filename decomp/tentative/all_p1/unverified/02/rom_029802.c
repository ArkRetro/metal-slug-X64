/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x029802.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_29802(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  if ((*(byte *)(unaff_A6 + 0x21) & 0x20) == 0) {
    unaff_A6[0x14] = 0x29b6e;
  }
  else {
    unaff_A6[0x14] = 0;
  }
  *(undefined2 *)((int)unaff_A6 + 0x4e) = *(undefined2 *)(unaff_A6[3] + 0x4e);
  candidate_1ad6e();
  candidate_9ea8();
  candidate_4520();
  *unaff_A6 = 0x29852;
  bVar3 = false;
  candidate_409a();
  if (!bVar3) {
    candidate_13f1e();
    candidate_4192();
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_1abc8();
  *(undefined1 *)(unaff_A6 + 0x21) = *(undefined1 *)(unaff_A6[3] + 0x84);
  bVar3 = false;
  if ((*(byte *)(unaff_A6 + 0x21) & 0x20) == 0) {
    candidate_299e6();
  }
  else {
    candidate_44930();
    if (bVar3) {
      candidate_13f1e();
    }
    else {
      candidate_299e6();
    }
  }
  bVar3 = false;
  if (unaff_A6[0x14] != 0) {
    candidate_4c24();
    if (bVar3) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      iVar2 = unaff_A6[3];
      *(undefined1 *)(iVar2 + 0x17) = *(undefined1 *)((int)unaff_A6 + 0x17);
      if (*(short *)((int)unaff_A6 + 0x4e) < *(short *)(iVar2 + 0x4e)) {
        *(undefined2 *)(iVar2 + 0x4e) = *(undefined2 *)((int)unaff_A6 + 0x4e);
      }
      else {
        *(undefined2 *)((int)unaff_A6 + 0x4e) = *(undefined2 *)(iVar2 + 0x4e);
      }
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
        *(byte *)(iVar2 + 0x5f) = *(byte *)(iVar2 + 0x5f) | 0x80;
      }
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = 0x298f4;
    }
  }
  return;
}
