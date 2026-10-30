/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09c46c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9c46c(void)

{
  int iVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  if ((*(byte *)(unaff_A6 + 0x24) & 0x40) == 0) {
    candidate_9ea8();
    candidate_4520();
  }
  else {
    candidate_9ea8();
    candidate_4520();
  }
  uVar2 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar2;
  candidate_9d3f0();
  candidate_1ad6e();
  candidate_4256();
  *unaff_A6 = 0x9c4de;
  if (*(char *)(unaff_A6[3] + 0x8d) != *(char *)((int)unaff_A6 + 0x8d)) {
    *(char *)((int)unaff_A6 + 0x8d) = *(char *)(unaff_A6[3] + 0x8d);
    candidate_4256();
  }
  candidate_1abc8();
  iVar1 = unaff_A6[3];
  *(undefined1 *)(unaff_A6 + 0x24) = *(undefined1 *)(iVar1 + 0x90);
  *(undefined2 *)((int)unaff_A6 + 0x26) = *(undefined2 *)(iVar1 + 0x26);
  uVar3 = 0;
  if ((*(byte *)(iVar1 + 0x15) & 1) == 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_427a();
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) == 0) {
    candidate_4c24();
    if ((bool)uVar3) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar3 = 0;
      *(byte *)(unaff_A6 + 0x24) = *(byte *)(unaff_A6 + 0x24) | 2;
    }
    else {
      *(byte *)(unaff_A6 + 0x24) = *(byte *)(unaff_A6 + 0x24) & 0xfd;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *(undefined4 *)unaff_A6[3] = 0x9b4a4;
      candidate_1ad52();
      unaff_A6[0x14] = 0;
      *unaff_A6 = candidate_9c5ba;
      uVar3 = 0;
    }
  }
  else {
    unaff_A6[0x14] = 0;
    *unaff_A6 = candidate_9c5ba;
    uVar3 = 0;
  }
  candidate_409a();
  if (!(bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
