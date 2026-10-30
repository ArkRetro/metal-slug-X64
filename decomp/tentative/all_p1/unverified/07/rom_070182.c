/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x070182.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_70182(void)

{
  byte bVar1;
  char cVar2;
  undefined1 extraout_D1b;
  undefined4 *unaff_A6;
  bool bVar3;
  undefined1 uVar4;
  undefined8 uVar5;

  candidate_710f8();
  *(undefined1 *)((int)unaff_A6 + 0x86) = 0;
  bVar3 = false;
  candidate_40bc();
  *(char *)(unaff_A6 + 0x18) = -!bVar3;
  candidate_1b242();
  *unaff_A6 = 0x701ac;
  if (*(char *)(unaff_A6 + 0x18) == '\0') {
    candidate_143ee();
    candidate_70db6();
  }
  else {
    candidate_4256();
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
    *unaff_A6 = 0x701dc;
    uVar4 = 0;
    candidate_143ee();
    candidate_70db6();
    if ((bool)uVar4) {
      *unaff_A6 = candidate_701f2;
    }
  }
  if (*(short *)((int)unaff_A6 + 0x7e) != 0) {
    *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  bVar1 = *(byte *)((int)unaff_A6 + 0x5f);
  *(byte *)((int)unaff_A6 + 0x5f) = bVar1 & 0xbf;
  if ((bVar1 & 0x40) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if (*(int *)((int)unaff_A6 + 0x6e) == 0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1b);
    if ((*(ushort *)((int)unaff_A6 + 0x7e) & 1) != 0) {
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
    }
  }
  else {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    candidate_7103e();
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  if ((DAT_0010f000 & 0x80) != 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  bVar3 = false;
  candidate_aa760();
  if (bVar3) {
    cVar2 = candidate_aaa88();
    if (cVar2 == '\t') {
      *(undefined1 *)((int)unaff_A6 + 0x8b) = extraout_D1b;
      *(undefined1 *)(unaff_A6 + 0x23) = 0;
    }
  }
  else {
    *unaff_A6 = 0x702f2;
  }
  if (*(char *)((int)unaff_A6 + 0x89) != '\0') {
    *(char *)((int)unaff_A6 + 0x89) = *(char *)((int)unaff_A6 + 0x89) + -1;
  }
  if (*(char *)((int)unaff_A6 + 0x8a) != '\0') {
    *(char *)((int)unaff_A6 + 0x8a) = *(char *)((int)unaff_A6 + 0x8a) + -1;
  }
  bVar3 = false;
  candidate_aa9e4();
  if (bVar3) {
    *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x1e;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
    uVar5 = candidate_2e7c();
    unaff_A6[0xc] = (int)((ulonglong)uVar5 >> 0x20) + unaff_A6[0xc];
    unaff_A6[0xd] = (int)uVar5 + unaff_A6[0xd];
    *unaff_A6 = candidate_70234;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_7026c;
  }
  *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + 1;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  return;
}
