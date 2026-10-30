/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7b0c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d7b0c(void)

{
  undefined2 uVar1;
  short sVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)(unaff_A6 + 0x22) = 0x84;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x76) = uVar1;
  candidate_d8f4e();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *unaff_A6 = 0xd76bc;
  bVar3 = false;
  candidate_d8fdc();
  if (bVar3) {
    candidate_d8f4e();
  }
  candidate_1ac38();
  candidate_d90d2();
  *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1;
  if (((*(short *)((int)unaff_A6 + 0x76) < 1) && (*(short *)(unaff_A6 + 0xf) < 0x111)) &&
     (*(char *)((int)unaff_A6 + 0x9a) == '\0')) {
    candidate_d9162();
    candidate_d914a();
  }
  sVar2 = candidate_260c8();
  if (*(short *)((int)unaff_A6 + 0x4e) <= sVar2) {
    *(undefined1 *)(unaff_A6 + 0x20) = 0xff;
  }
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 0;
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    if (((*(short *)(unaff_A6 + 0xf) < 0x90) ||
        (bVar3 = *(ushort *)(unaff_A6 + 0xf) < 0x120, 0x120 < (short)*(ushort *)(unaff_A6 + 0xf)))
       || (candidate_1af9a(), bVar3)) {
      *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    }
    else {
      *unaff_A6 = candidate_d7d7c;
    }
  }
  return;
}
