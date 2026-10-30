/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x087066.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_87066(void)

{
  char cVar1;
  int iVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  unaff_A6[0x14] = 0x87c92;
  if ((*(byte *)((int)unaff_A6 + 0x8b) & 4) != 0) {
    candidate_4256();
    *unaff_A6 = 0x8708e;
    uVar3 = 0;
    candidate_1436a();
    candidate_427a();
    candidate_87b2e();
    if ((bool)uVar3) {
      *unaff_A6 = candidate_86f4c;
      uVar3 = 0;
    }
    candidate_87b64();
    if ((bool)uVar3) {
      *unaff_A6 = 0x86b0e;
    }
    goto LAB_0008712e;
  }
  unaff_A6[0x14] = 0x87c92;
  if (*(short *)((int)unaff_A6 + 0x7e) < *(short *)(unaff_A6 + 0xf)) {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
LAB_00086b0e:
      unaff_A6[0x14] = 0x87c92;
      candidate_4256();
      *(undefined1 *)((int)unaff_A6 + 0x87) = 6;
      *unaff_A6 = 0x86b32;
      uVar3 = 0;
      candidate_1436a();
      candidate_427a();
      if ((bool)uVar3) {
        *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
        *unaff_A6 = candidate_86a1c;
      }
      goto LAB_0008712e;
    }
  }
  else if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) goto LAB_00086b0e;
  iVar2 = *(int *)((int)unaff_A6 + 0x76);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar2 = -iVar2;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar2;
  candidate_4256();
  *unaff_A6 = 0x87110;
  uVar3 = 0;
  candidate_1456a();
  candidate_427a();
  candidate_87b3a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_87066;
  }
LAB_0008712e:
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = 0x86eb4;
  }
  cVar1 = *(char *)((int)unaff_A6 + 0x87);
  *(char *)((int)unaff_A6 + 0x87) = cVar1 + -1;
  if (cVar1 == '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0;
  }
  *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
  if (*(short *)(unaff_A6 + 0x20) < 0) {
    *(undefined2 *)(unaff_A6 + 0x20) = 0;
  }
  bVar4 = false;
  candidate_4c24();
  if ((bVar4) && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_871aa;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_873c0;
  }
  bVar4 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_873c0;
  }
  return;
}
