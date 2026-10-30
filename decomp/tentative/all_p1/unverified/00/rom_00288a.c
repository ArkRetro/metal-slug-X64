/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x00288a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_288a(void)

{
  short sVar1;
  short in_D0w;
  short in_D1w;
  short unaff_D2w;
  short unaff_D3w;
  short *in_A0;
  short *in_A1;

  sVar1 = (*in_A1 + unaff_D2w) - (*in_A0 + in_D0w);
  if (sVar1 < 0) {
    if ((short)(in_A1[2] + sVar1) < 0) {
      return;
    }
    sVar1 = (in_A1[1] + unaff_D3w) - (in_A0[1] + in_D1w);
    if (-1 < sVar1) {
      if (-1 < (short)(sVar1 - in_A0[3])) {
        return;
      }
      return;
    }
    if ((short)(in_A1[3] + sVar1) < 0) {
      return;
    }
    return;
  }
  if (-1 < (short)(sVar1 - in_A0[2])) {
    return;
  }
  sVar1 = (in_A1[1] + unaff_D3w) - (in_A0[1] + in_D1w);
  if (-1 < sVar1) {
    if (-1 < (short)(sVar1 - in_A0[3])) {
      return;
    }
    return;
  }
  if ((short)(in_A1[3] + sVar1) < 0) {
    return;
  }
  return;
}
