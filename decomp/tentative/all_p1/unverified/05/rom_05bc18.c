/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05bc18.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5bc18(void)

{
  char cVar1;
  undefined2 uVar3;
  short sVar4;
  int iVar2;
  undefined4 *unaff_A6;
  bool bVar5;
  undefined1 uVar6;

  candidate_1ad6e();
  candidate_9ea8();
  candidate_4520();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar3;
  sVar4 = candidate_260c8();
  iVar2 = sVar4 * 0x100;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar2 = sVar4 * -0x100;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar2;
  unaff_A6[0x14] = 0x5c2e6;
  *unaff_A6 = 0x5bc9c;
  bVar5 = false;
  candidate_5c176();
  if (bVar5) {
    candidate_4256();
    *unaff_A6 = 0x5bcea;
    uVar6 = 0;
    candidate_1abc8();
    candidate_5c152();
    candidate_427a();
    if ((bool)uVar6) {
      *unaff_A6 = candidate_5bd1c;
      if ((*(byte *)(unaff_A6 + 0x21) & 1) != 0) {
        *unaff_A6 = candidate_5be16;
      }
      candidate_1ad52();
    }
  }
  else {
    candidate_1abc8();
  }
  cVar1 = *(char *)(unaff_A6[3] + 0x60);
  if (cVar1 == '\x01') {
    *unaff_A6 = candidate_4188;
  }
  bVar5 = cVar1 == '\0';
  candidate_1945c();
  if (bVar5) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
