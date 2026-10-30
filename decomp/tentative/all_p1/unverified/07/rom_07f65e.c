/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07f65e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7f65e(void)

{
  char cVar1;
  char cVar2;
  int iVar3;
  undefined4 *unaff_A6;
  undefined8 uVar4;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
  candidate_4256();
  candidate_9ea8();
  uVar4 = candidate_2e7c();
  *(int *)((int)unaff_A6 + 0x76) = (int)((ulonglong)uVar4 >> 0x20);
  unaff_A6[0xd] = (int)uVar4;
  candidate_1aca2();
  *(undefined1 *)((int)unaff_A6 + 0x86) = 2;
  *unaff_A6 = 0x7f6b2;
  *(undefined4 *)((int)unaff_A6 + 0x72) = unaff_A6[0xd];
  unaff_A6[0xc] = *(int *)(unaff_A6[3] + 0x30) + *(int *)((int)unaff_A6 + 0x76);
  candidate_143dc();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    unaff_A6[0xc] = (int)unaff_A6[0xc] >> 2;
    iVar3 = -(*(int *)((int)unaff_A6 + 0x72) >> 1);
    unaff_A6[0xd] = iVar3;
    *(int *)((int)unaff_A6 + 0x72) = iVar3;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
    cVar1 = *(char *)((int)unaff_A6 + 0x86);
    cVar2 = cVar1 + -1;
    *(char *)((int)unaff_A6 + 0x86) = cVar2;
    if (cVar2 == '\0' || cVar1 < '\x01') {
      *unaff_A6 = 0x7f712;
    }
  }
  candidate_427a();
  candidate_7f7b8();
  return;
}
